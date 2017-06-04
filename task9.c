#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <sys/loadavg.h>

#include "extern.h"

info_t info;

info_t get_info(){
	info_t info;

	info.pid = getpid();
	info.uid = getuid();
	info.gid = getgid();

	info.diff = 0;

	getloadavg(info.avg, 3);

	return info;
}

void sighandler(int signo){
	switch (signo) {
		case SIGINT:
			printf("%ld\n", (long) info.uid);
			break;
		case SIGHUP:
			printf("%ld\n", (long) info.pid);
			break;
		case SIGTERM:
			printf("%ld\n", (long) info.gid);
			break;
		case SIGUSR1:
			printf("%ld\n", (long) info.diff);
			break;
		case SIGUSR2:
			for (int i = 0; i < 3; i++)
				printf("%.6f ", info.avg[i]);
			printf("\n");
			break;
	}
}

int main(){
	time_t start = time(NULL);

	info = get_info();

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sighandler;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	while (1) {
		time_t cur_time = time(NULL);

		info.diff = cur_time - start;
		getloadavg(info.avg, 3);
		sleep(1);
	}

	return EXIT_SUCCESS;
}
