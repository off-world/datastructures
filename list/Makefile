CFLAGS = -std=c99 -pedantic -Wall -O2 -fpic -c
LDFLAGS = -shared
CC = gcc

.PHONY: all
all: list

list: list.o
	$(CC) $(LDFLAGS) -o libll.so list.o

list.o: list.c list.h
	$(CC) $(CFLAGS) list.c

.PHONY: clean
clean:
	rm *.o

