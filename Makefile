CC=gcc
CFLAGS=-std=c99 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE -Wall -Wextra -pedantic -c

all: part1 part2 part3

part1: task1 task2 task3

part2: task4 task5 task6 task7

part3: task8 task9 task10

task1: task1_server task1_client

task2: task2_server task2_client

task3: task3_server task3_client

task1_server: task1_server.o
	$(CC) $^ -o $@

task1_client: task1_client.o
	$(CC) $^ -o $@

task1_server.o: task1_server.c
	$(CC) $(CFLAGS) $^ -o $@

task1_client.o: task1_client.c
	$(CC) $(CFLAGS) $^ -o $@

task2_server: task2_server.o
	$(CC) $^ -o $@

task2_client: task2_client.o
	$(CC) $^ -o $@

task2_server.o: task2_server.c
	$(CC) $(CFLAGS) $^ -o $@

task2_client.o: task2_client.c
	$(CC) $(CFLAGS) $^ -o $@

task3_server.o: task3_server.c
	$(CC) $(CFLAGS) $^ -o $@

task3_client.o: task3_client.c
	$(CC) $(CFLAGS) $^ -o $@

task4: task4.o
	$(CC) $^ -lrt -o $@

task4.o: task4.c
	$(CC) $(CFLAGS) $^ -o $@

task5: task5.o
	$(CC) $^ -lrt -o $@

task5.o: task5.c
	$(CC) $(CFLAGS) $^ -o $@

task6: task6.o
	$(CC) $^ -lpthread -o $@

task6.o: task6.c
	$(CC) $(CFLAGS) $^ -o $@

task7: task7.o
	$(CC) $^ -lpthread -o $@

task7.o: task7.c
	$(CC) $(CFLAGS) $^ -o $@

task8: task8_server task8_client

task8_server: task8_server.o
	$(CC) $^ -lnsl -lsocket -o $@

task8_client: task8_client.o
	$(CC) $^ -lnsl -lsocket -o $@

task8_server.o: task8_server.c
	$(CC) $(CFLAGS)  $^ -o $@

task8_client.o: task8_client.c
	$(CC) $(CFLAGS) $^ -o $@

task9: task9.o
	$(CC) $^ -o $@

task9.o: task9.c
	$(CC) $(CFLAGS) $^ -o $@

task10: task10.o
	$(CC) $^ -o $@

task10.o: task10.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o task{1..10}{_client,server,''}
