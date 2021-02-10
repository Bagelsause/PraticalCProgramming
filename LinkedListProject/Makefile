#CC=gcc <- debugging purposes
CC=clang
CFLAGS=-std=gnu11 -Wall

main: main.c llist.c
	$(CC) $(CFLAGS) llist.c main.c -o main

test: test.c llist.c
	$(CC) $(CFLAGS) llist.c test.c -o test
