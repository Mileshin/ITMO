#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/loadavg.h>

#include "extern.h"

int msg_id;

info_t get_info(){
	info_t info;

	info.pid = getpid();
	info.uid = getuid();
	info.gid = getgid();

	info.diff = 0;

	getloadavg(info.avg, AVG_LEN);

	return info;
}

void sighandler(int signo){
	(void) signo;
	msgctl(msg_id, IPC_RMID, NULL);
	_exit(0);
}

int main(){
	time_t start = time(NULL);
	time_t sur_time; 
	struct sigaction act;
	key_t key = KEY;
	msg_t msg;

	memset(&act, 0, sizeof(act));
	act.sa_handler = sighandler;
	sigaction(SIGINT, &act, NULL);

	/* setup message queue */
	msg_id = msgget(key, IPC_CREAT | 0644);
	if (msg_id < 0) {
		perror("msgget");
		return EXIT_FAILURE;
	}

	msg.info = get_info();
	msg.type = 1;

	while (1) {
		cur_time = time(NULL);

		if (msgsnd(msg_id, &msg, sizeof(msg), 0) < 0) {
			perror("msgsnd");
		}

		msg.type++;
		msg.info.diff = cur_time - start_time;
		getloadavg(msg.info.avg, AVG_LEN);

		sleep(1);
	}

	return EXIT_SUCCESS;
}
