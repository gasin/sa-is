#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 10000000 //max length of string (> VAR)
#define VAR 256
#define INT_SIZE 4
#define LS_FLAG (1<<29)
#define LS_MASK ((1<<29)-1)
#define LMS_FLAG (1<<30)

int buf[SIZE+10];
int pos[SIZE+10];

#define init_pos \
    memset(pos, 0, (var+1) * INT_SIZE); \
    for (int i = 0; i < si; i++) { pos[*(buf_st+i)+1]++; } \
    for (int i = 1; i < var; i++) { pos[i+1] += pos[i]; } 
    
#define induced_sort \
    for (int i = 0; i < si; i++) if (SA[i] > 0 && (*(buf_en+SA[i]-1) & LS_FLAG)) { \
        SA[pos[*(buf_st+SA[i]-1)]++] = SA[i]-1; \
    } \
    for (int i = 0; i < si; i++) if (SA[i] > 0 && (*(buf_en+SA[i]-1) & LS_FLAG)) { \
        pos[*(buf_st+SA[i]-1)]--; \
    } \
    for (int i = si-1; i >= 1; i--) if (SA[i] > 0 && !(*(buf_en+SA[i]-1) & LS_FLAG)) { \
        SA[--pos[*(buf_st+SA[i]-1)+1]] = SA[i]-1; \
    }

void SA_IS_int(int var, int i_st, int i_en, int *SA)
{
    if (i_en - i_st == 1) {
        SA[1] = 0;
        return;
    }
    buf[i_en++] = 0;
    int si = i_en - i_st;
    int *buf_st = buf + i_st, *buf_en = buf + i_en;
    
    memset(buf_en, 0, si * INT_SIZE);
    
    for (int i = si-2; i >= 0; i--) { 
        if (*(buf_st+i) == *(buf_st+i+1)) { 
            *(buf_en+i) = *(buf_en+i+1); 
            continue; 
        } 
        if (*(buf_st+i) > *(buf_st+i+1)) { 
            *(buf_en+i) |= LS_FLAG; 
        } 
    } 
    
    init_pos;
    
    int next_si = 0;
    
    memset(SA, 0, si * INT_SIZE);
    for (int i = 1; i < si; i++) {
        if (!(*(buf_en+i) & LS_FLAG) && (*(buf_en+i-1) & LS_FLAG)) {
            SA[--pos[*(buf_st+i)+1]] = i;
            *(buf_en+i) |= LMS_FLAG;
            *(buf_en+i) += i_en+next_si++;
        }
    }
    for (int i = 1; i < si; i++) if (*(buf_en+i) & LMS_FLAG) { 
        pos[*(buf_st+i)+1]++; 
    }
    
    induced_sort;
    
    for (int i = 1; i < si; i++) if (*(buf_en+i) & LMS_FLAG) {
        pos[i] = *(buf_en+i) & LS_MASK;
    }    

    int counter = 0;
    int pre_sa = -1;
    for (int i = 0; i < si; i++) if (*(buf_en+SA[i]) & LMS_FLAG) {
        int k = SA[i];
        buf[pos[k]] = (buf[pos[k]] & LMS_FLAG) + (++counter);
        
        if (pre_sa == -1 || *(buf_st+k) != *(buf_st+pre_sa)) {
            pre_sa = k;
            continue;
        }
        for (int j = 1; i+j < si; j++) {
            if (*(buf_en+k+j) & LMS_FLAG) {
                if ((*(buf_en+pre_sa+j) & LMS_FLAG) && *(buf_st+k+j) == *(buf_st+pre_sa+j)) {
                    buf[pos[k]]--;
                    --counter;
                }
                break;
            }
            if ((*(buf_en+pre_sa+j) & LMS_FLAG) || *(buf_st+k+j) != *(buf_st+pre_sa+j)) {
                break;
            }
        }
        pre_sa = k;
    }
    
    for (int i = 0; i < next_si; i++) { *(buf_en+i) &= LS_MASK; }
    
    SA_IS_int(counter+1, i_en, i_en+next_si, SA);
    
    memset(buf_en, 0, si * INT_SIZE);
    
    for (int i = si-2; i >= 0; i--) { 
        if (*(buf_st+i) == *(buf_st+i+1)) { 
            *(buf_en+i) = *(buf_en+i+1); 
            continue; 
        } 
        if (*(buf_st+i) > *(buf_st+i+1)) { 
            *(buf_en+i) |= LS_FLAG; 
        } 
    } 
    
    init_pos;
    
    next_si = 0;
    for (int i = 1; i < si; i++) {
        if (!(*(buf_en+i) & LS_FLAG) && (*(buf_en+i-1) & LS_FLAG)) {
            *(buf_en+next_si++) += i;
        }
    }
    
    for (int i = 0; i < next_si; i++) { *(buf_en+next_si+i) += (*(buf_en+SA[1+i]) & LS_MASK); }
    for (int i = 0; i < next_si; i++) { *(buf_en+i) = (*(buf_en+i) & LS_FLAG) + *(buf_st+(*(buf_en+next_si+i) & LS_MASK))+1; }
    
    memset(SA, 0, si * INT_SIZE);
    
    for (int i = next_si-1; i >= 0; i--) { 
        SA[--pos[*(buf_en+i) & LS_MASK]] = *(buf_en+next_si+i) & LS_MASK; 
    }
    
    for (int i = next_si-1; i >= 0; i--) { 
        pos[*(buf_en+i) & LS_MASK]++;
    }
    
    induced_sort;
}

void SA_IS_char(int var, int si, int *SA, char *input)
{
    if (si == 1) {
        SA[1] = 0;
        return;
    }
    si++;
    
    memset(buf, 0, si * INT_SIZE);
    
    for (int i = si-2; i >= 0; i--) { 
        if (input[i] == input[i+1]) { 
            buf[i] = buf[i+1];
        } else if(input[i] > input[i+1]) {
            buf[i] |= LS_FLAG;
        }
    } 
    
    memset(pos, 0, (var+1) * INT_SIZE); 
    for (int i = 0; i < si; i++) { pos[input[i]+1]++; } 
    for (int i = 1; i < var; i++) { pos[i+1] += pos[i]; } 
    
    int next_si = 0;
    
    memset(SA, 0, si * INT_SIZE);
    for (int i = 1; i < si; i++) {
        if (!(buf[i] & LS_FLAG) && (buf[i-1] & LS_FLAG)) {
            SA[--pos[input[i]+1]] = i;
            buf[i] |= LMS_FLAG;
            buf[i] += next_si++;
        }
    }
    for (int i = 1; i < si; i++) if (buf[i] & LMS_FLAG) { 
        pos[input[i]+1]++; 
    }
    
    for (int i = 0; i < si; i++) if (SA[i] > 0 && (buf[SA[i]-1] & LS_FLAG)) { 
        SA[pos[input[SA[i]-1]]++] = SA[i]-1; 
    } 
    for (int i = 0; i < si; i++) if (SA[i] > 0 && (buf[SA[i]-1] & LS_FLAG)) { 
        pos[input[SA[i]-1]]--; 
    } 
    for (int i = si-1; i >= 1; i--) if (SA[i] > 0 && !(buf[SA[i]-1] & LS_FLAG)) { 
        SA[--pos[input[SA[i]-1]+1]] = SA[i]-1; 
    }
    
    for (int i = 1; i < si; i++) if (buf[i] & LMS_FLAG) {
        pos[i] = buf[i] & LS_MASK;
    }    

    int counter = 0;
    int pre_sa = -1;
    for (int i = 0; i < si; i++) if (buf[SA[i]] & LMS_FLAG) {
        int k = SA[i];
        buf[pos[k]] = (buf[pos[k]] & (LS_FLAG | LMS_FLAG)) + (++counter);
        
        if (pre_sa == -1 || input[k] != input[pre_sa]) {
            pre_sa = k;
            continue;
        }
        for (int j = 1; i+j < si; j++) {
            if (buf[k+j] & LMS_FLAG) {
                if ((buf[pre_sa+j] & LMS_FLAG) && input[k+j] == input[pre_sa+j]) {
                    buf[pos[k]]--;
                    --counter;
                }
                break;
            }
            if ((buf[pre_sa+j] & LMS_FLAG) || input[k+j] != input[pre_sa+j]) {
                break;
            }
        }
        pre_sa = k;
    }
    
    for (int i = 0; i < next_si; i++) { buf[i] &= LS_MASK; }
    
    SA_IS_int(counter+1, 0, next_si, SA);
    
    memset(buf, 0, si * INT_SIZE);
    
    for (int i = si-2; i >= 0; i--) { 
        if (input[i] == input[i+1]) { 
            buf[i] = buf[i+1]; 
        } else if (input[i] > input[i+1]) {
            buf[i] |= LS_FLAG;
        }
    } 
    
    memset(pos, 0, (var+1) * INT_SIZE); 
    for (int i = 0; i < si; i++) { pos[input[i]+1]++; } 
    for (int i = 1; i < var; i++) { pos[i+1] += pos[i]; } 
    
    next_si = 0;
    for (int i = 1; i < si; i++) {
        if (!(buf[i] & LS_FLAG) && (buf[i-1] & LS_FLAG)) {
            buf[i] |= LMS_FLAG;
            buf[next_si++] += i;
        }
    }
    
    for (int i = 0; i < next_si; i++) { buf[next_si+i] += (buf[SA[1+i]] & LS_MASK); }
    for (int i = 0; i < next_si; i++) { buf[i] = (buf[i] & (LS_FLAG|LMS_FLAG)) + input[buf[next_si+i] & LS_MASK]+1; }
    
    memset(SA, 0, si * INT_SIZE);
    
    for (int i = next_si-1; i >= 0; i--) { 
        SA[--pos[buf[i] & LS_MASK]] = buf[next_si+i] & LS_MASK; 
    }
    
    for (int i = next_si-1; i >= 0; i--) { 
        pos[buf[i] & LS_MASK]++;
    }
    
    for (int i = 0; i < si; i++) if (SA[i] > 0 && (buf[SA[i]-1] & LS_FLAG)) { 
        SA[pos[input[SA[i]-1]]++] = SA[i]-1; 
    } 
    for (int i = 0; i < si; i++) if (SA[i] > 0 && (buf[SA[i]-1] & LS_FLAG)) { 
        pos[input[SA[i]-1]]--; 
    } 
    for (int i = si-1; i >= 1; i--) if (SA[i] > 0 && !(buf[SA[i]-1] & LS_FLAG)) { 
        SA[--pos[input[SA[i]-1]+1]] = SA[i]-1; 
    }
}

void solve_sa_is(char *input, int *SA) 
{
    int si = strlen(input);
    if (si == 0) {
        return;
    }
    
    if (si <= VAR) { //compress index of characters
        int press[VAR] = {0};
        for (int i = 0; i < si; i++) { press[buf[i]]++; }
        int counter = 0;
        for (int i = 0; i < VAR; i++) if (press[i]) { 
            press[i] = ++counter; 
        }
        for (int i = 0; i < si; i++) { buf[i] = press[buf[i]]; }
        SA_IS_int(counter+1, 0, si, SA);
    } else {
        SA_IS_char(VAR, si, SA, input);
    }
}

int verification (char *input, int *SA, int si)
{
    for (int i = 1; i <= si; i++) {
        if (SA[i] >= si) { return 0; }
        for (int j = i+1; j <= si; j++) {
            if (SA[i] == SA[j]) { return 0; }
        }
    }
    for (int i = 1; i < si; i++) {
        if (strcmp(input + SA[i], input + SA[i+1]) > 0) {
            return 0;
        }
    }
    return 1;
}

char input[SIZE+10];
int SA[SIZE+10]; //[1..SIZE]

int main(int argv, char *argc[])
{
    FILE *fp;
    
    if (argv != 3) {
        printf("Usage : ./sa-is.exe (path to input) (path to output)\n");
        exit(EXIT_FAILURE);
    }
    
    if ((fp = fopen(argc[1], "r")) == NULL) {
        fprintf(stderr, "failed to open %s.\n", argc[1]);
        exit(EXIT_FAILURE);
    }
    fscanf(fp, "%s", input);
    fclose(fp);
    clock_t start = clock();
    solve_sa_is(input, SA);
    clock_t end = clock();
    
    int si = strlen(input);
    printf("size ... %d\n", si);
    printf("sec  ... %.3f\n", (double)(end-start)/CLOCKS_PER_SEC);
    
    if (si <= 5000) {
        if (verification(input, SA, si)) {
            printf("verification succeeded.\n");
        } else {
            printf("verification failed.\n");
        }
    } else {
        printf("run verification iff input size <= 5000.\n");
    }
    
    if ((fp = fopen(argc[2], "w")) == NULL) {
        fprintf(stderr, "failed to open %s.\n", argc[2]);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < si; i++) {
        fprintf(fp, "%d", SA[i+1]);
        if (i == si-1) {
            fprintf(fp, "\n");
        } else {
            fprintf(fp, " ");
        }
    }
    fclose(fp);
}

