CC=gcc
CFLAGS=-std=c99 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE -Wall -Wextra -pedantic -c

all: part1

part1: task1

task1: task1_server task1_client

task1_server: task1_server.o
	$(CC) $^ -o $@

task1_client: task1_client.o
	$(CC) $^ -o $@

task1_server.o: task1_server.c
	$(CC) $(CFLAGS) $^ -o $@

task1_client.o: task1_clent.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o task{1..10}{_client,server,''}
