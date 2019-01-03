CFLAGS = -std=c99 -pedantic -Wall -O2 -fpic -c
LDFLAGS = -shared
CC = gcc

.PHONY: all
all: hashmap

hashmap: hashmap.o
	$(CC) $(LDFLAGS) -o libhm.so hashmap.o

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) hashmap.c

.PHONY: clean
clean:
	rm *.o

