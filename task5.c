#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread.h>
#include <signal.h>
#include <semaphore.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#include "extern.h"

#define ALPHABET_LEN 26

#define SET_LOCK(X,Y) struct sembuf lock = {X, Y, SEM_UNDO}

#define SET_UNLOCK(X,Y) struct sembuf unlock = {X, Y, SEM_UNDO}

char alphabet[ALPHABET_LEN] = "abcdefghijklmnopqrstuvwxyz";

thread_t t[2];

key_t key = KEY;
int	semid;

void print_alphabet(){
	for (int i = 0; i < ALPHABET_LEN; i++) {
		printf("%c", alphabet[i]);
	}
	printf("\n");
}

void *reverse(void *arg){
	(void) arg;
	
	SET_LOCK(0, -1);
	SET_UNLOCK(2, 1);

	while (semop(semid, &lock, 1) != -1) {
		for (int i = 0; i < ALPHABET_LEN / 2; i++) {
			char t = alphabet[i];
			alphabet[i] = alphabet[ALPHABET_LEN - i - 1];
			alphabet[ALPHABET_LEN - i - 1] = t;
		}
		semop(semid, &unlock, 1);
	}
	return NULL;
}

void *changecase(void *arg){
	(void) arg;

	SET_LOCK(1, -1);
	SET_UNLOCK(2, 1);

	while (semop(semid, &lock, 1) != -1) {
		for (int i = 0; i < ALPHABET_LEN; i++) {
			alphabet[i] += (alphabet[i] - 'A') < ALPHABET_LEN ? 32 : -32;
		}
		semop(semid, &unlock, 1);
	}
	return NULL;
}

void sighandler(int signo){
	(void) signo;
	semctl(semid, 0, IPC_RMID);
	_exit(0);
}

int main(){
	struct sigaction act;
        memset(&act, 0, sizeof(act));
        act.sa_handler = sighandler;
        sigaction(SIGINT, &act, NULL);

	key_t key = KEY;

	int sem_init[3] = {0, 0, 0};
	/* create 3 semaphores and get their id */
	semid = semget(key, 3, IPC_CREAT | 0644);
	/* init all semaohores */
	semctl(semid, 0, GETALL, sem_init);
	
	thr_create(NULL, NULL, reverse, NULL, NULL, &t[0]);
	thr_create(NULL, NULL, ccase, NULL, NULL, &t[1]);

	struct sembuf sem_action = {0,  1, SEM_UNDO};
	struct sembuf sem_print  = {2, -1, SEM_UNDO};

	while (1) {
		sem_action.sem_num = (sem_action.sem_num + 1) % 2;
		semop(semid, &sem_action, 1);
		semop(semid, &sem_print, 1);
		print_alphabet();
		sleep(1);
	}

	return EXIT_SUCCESS;
}
