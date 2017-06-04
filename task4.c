#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <thread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_LEN 26

char alphabet[ALPHABET_LEN] = "abcdefghijklmnopqrstuvwxyz";

thread_t 	t[2];
sem_t 		sem[3];

void print_alpha(){
	for (int i = 0; i < ALPHABET_LEN; i++) {
		printf("%c", alpha[i]);
	}
	printf("\n");
}

void *reverse(void *arg){
	(void) arg;
	while (sem_wait(&sem[0]) == 0) {
		for (int i = 0; i < ALPHABET_LEN / 2; i++) {
			char t = alpha[i];
			alpha[i] = alpha[ALPHABET_LEN - i - 1];
			alpha[ALPHABET_LEN - i - 1] = t;
		}
		sem_post(&sem[2]);
	}
	return NULL;
}

void *change_case(void *arg){
	(void) arg;
	while (sem_wait(&sem[1]) == 0) {
		for (int i = 0; i < LETTERS; i++) {
			alpha[i] += (alpha[i] - 'A') < LETTERS ? 32 : -32;
		}
		sem_post(&sem[2]);
	}
	return NULL;
}

void sighandler(int signo){
	(void) signo;
	for (int i = 0; i < 3; i++) {
		sem_destroy(&sem[i]);
	}
	_exit(0);
}

int main(){
	struct sigaction act;
        memset(&act, 0, sizeof(a));
        act.sa_handler = sighandler;
        sigaction(SIGINT, &a, NULL);

	for (int i = 0; i < 3; i++) {
		if (sem_init(&sem[i], 0, 0) == -1) {
			perror("sem_init");
			return EXIT_FAILURE;
		}
	}
	/* Creating a new threads */
	thr_create(NULL, NULL, reverse, NULL, NULL, &t[0]);
	thr_create(NULL, NULL, ccase, NULL, NULL, &t[1]);

	while (1) {
		unsigned int i = (i + 1) % 2;
		sem_post(&sem[i]);
		sem_wait(&sem[2]);
		print_alpha();
		sleep(1);
	}

	return EXIT_SUCCESS;
}
