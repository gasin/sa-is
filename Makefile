#Makefile
all: sa_is make_test
sa_is: sa_is.c
	gcc -o sa_is sa_is.c -O3
make_test: make_test.c
	gcc -o make_test make_test.c -O3
