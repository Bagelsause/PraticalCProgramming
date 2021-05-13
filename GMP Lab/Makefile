CC=clang
CFLAGS=-std=gnu11 -Wall $(shell pkg-config --cflags --libs gmp) -Os
ASAN_CFLAGS=-O1 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -g

main: main.c
	$(CC) $(CFLAGS) main.c -o main

debug: main.c
	$(CC) $(CFLAGS) $(ASAN_CFLAGS) main.c -o main
