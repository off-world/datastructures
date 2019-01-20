CFLAGS = -std=c99 -pedantic -Wall -O2 -fpic -c
LDFLAGS = -shared
CC = gcc

.PHONY: all
all: stack

stack: stack.o
	$(CC) $(LDFLAGS) -o libstack.so stack.o

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) stack.c

.PHONY: clean
clean:
	rm *.o

