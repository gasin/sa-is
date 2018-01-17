#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argv, char *argc[])
{
    if (argv != 3) {
        printf("Usage : ./make_test (path to output) (length of string)\n");
        exit(EXIT_FAILURE);
    }
    FILE *fp;
    if ((fp = fopen(argc[1], "w")) == NULL) {
        fprintf(stderr, "failed to open %s.\n", argc[1]);
        exit(EXIT_FAILURE);
    }
    srand((unsigned)time(NULL));
    int si = atoi(argc[2]);
    for (int i = 0; i < si; i++) {
        fprintf(fp, "%c", 'a'+rand()%26);
    }
    fprintf(fp, "\n");
    fclose(fp);
}
