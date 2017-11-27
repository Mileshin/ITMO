#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "common.h"

#define DELAY 500000 

#define ALPHABET_LEN 26

char alphabet[ALPHABET_LEN] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t[3];
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void print_alphabet(){
	for (int i = 0; i < ALPHABET_LEN; i++) {
		printf("%c", alphabet[i]);
	}
	printf("\n");
}

void *print_uppercase_count(void *arg)
{
	(void) arg;

	while (1) {
		pthread_rwlock_rdlock(&rwlock);
		int k = 0;
		for (int i = 0; i < ALPHABET_LEN; i++) {
			if ('A' <= alphabet[i] && alphabet[i] <= 'Z')
				k++;
		}
		printf("%i\n", k);
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
	return NULL;
}

void *reverse(void *arg){
	(void) arg;

	while (1) {
		pthread_rwlock_wrlock(&rwlock);
		pthread_rwlock_rdlock(&rwlock);
		for (int i = 0; i < ALPHABET_LEN / 2; i++) {
			char t = alphabet[i];
			alphabet[i] = alphabet[ALPHABET_LEN - i - 1];
			alphabet[ALPHABET_LEN - i - 1] = t;
		}
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
	return NULL;
}

void *changecase(void *arg){
	(void) arg;

	while (1) {
		pthread_rwlock_wrlock(&rwlock);
		pthread_rwlock_rdlock(&rwlock);
		for (int i = 0; i < ALPHABET_LEN; i++) {
			alphabet[i] += (alphabet[i] - 'A') < ALPHABET_LEN ? 32 : -32;
		}
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
	return NULL;
}

int main(){
	pthread_create(&t[0], NULL, reverse, NULL);
	pthread_create(&t[1], NULL, changecase, NULL);
	pthread_create(&t[2], NULL, print_uppercase_count, NULL);

	while (1) {
		pthread_rwlock_rdlock(&rwlock);
		print_alphabet();
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}

	return EXIT_SUCCESS;
}
