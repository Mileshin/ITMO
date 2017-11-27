#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "extern.h"

#define DELAY 500000 

#define ALPHABET_LEN 26

char alphabet[ALPHABET_LEN] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void print_alphabet(){
	for (int i = 0; i < ALPHABET_LEN; i++) {
		printf("%c", alphabet[i]);
	}
	printf("\n");
}

void *reverse(void *arg){
	(void) arg;

	while (1) {
		pthread_mutex_lock(&mutex);
		for (int i = 0; i < ALPHABET_LEN / 2; i++) {
			char t = alphabet[i];
			alphabet[i] = alphabet[ALPHABET_LEN - i - 1];
			alphabet[ALPHABET_LEN - i - 1] = t;
		}
		pthread_mutex_unlock(&mutex);
		usleep(DELAY);
	}
	return NULL;
}

void *changecase(void *arg)
{
	(void) arg;

	while (1) {
		pthread_mutex_lock(&mutex);
		for (int i = 0; i < ALPHABET_LEN; i++) {
			alphabet[i] += (alphabet[i] - 'A') < ALPHABET_LEN ? 32 : -32;
		}
		pthread_mutex_unlock(&mutex);
		usleep(DELAY);
	}
	return NULL;
}

int main()
{
	pthread_create(&t[0], NULL, reverse, NULL);
	pthread_create(&t[1], NULL, changecase, NULL);

	while (1) {
		pthread_mutex_lock(&mutex);
		print_alphabet();
		pthread_mutex_unlock(&mutex);
		usleep(DELAY);
	}

	return EXIT_SUCCESS;
}
