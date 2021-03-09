CC=clang
CFLAGS=-std=gnu11 -Wall
ASAN_CFLAGS=-O1 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -g

main: kvp_list.c hashtable.c main.c
	$(CC) $(CFLAGS) $(ASAN_CFLAGS) kvp_list.c hashtable.c main.c -o main

test: kvp_list.c hashtable.c test.c
	$(CC) $(CFLAGS) $(ASAN_CFLAGS) kvp_list.c hashtable.c test.c -o test
