CC=clang
CFLAGS=-std=gnu11 -Wall $(shell pkg-config --cflags) $(shell curl-config --libs) -Os -lavcodec -lavformat -lavutil -lswresample -lchromaprint 

main: main.c
	$(CC) $(CFLAGS) main.c -o main
