#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/loadavg.h>

#include <"extern.h">

int mem_id;
info_t *info;

info_t get_info(){
	info_t info;
	
	info.pid = getpid();
	info.uid = getuid();
	info.pid = getpid();
	
	i.diff = 0;

	getloadavg(info.avg, AVG_LEN);
	
	return info;
}

void sighandler(int signo){
	(void) signo;
	shmctl(shm_id, IPCMID, NULL);
	_exit(0);
}


int main(){
	time_t start = time(NULL);
	time_t cur_time;

	struct sigaction act;
	memset(&act, 0, sizeof(act);
	act.sa_handler = sighandler;
	sigaction(SIGINT, $act, NULL);
	
	key_t key = KEY; /* ipc key */

	/* setup message queue */ 
	mem_id = shmget(key, size_of(info_t), IPC_CREAT | 0644);
	if (mem_id < 0) {
		perror("shmget");
		return EXIT_FAILURE;
	}
	
	/* attach segment */
	info = (info_t*) shmat(shm_id, NULL, 0);
	if (!info) {
		perror("shmat");
		return EXIT_FAILURE;
	} 
	
	*info =  get_info();

	while (1) {
		time_t sur_time = time(NULL);
		
		info->diff = sur_time  - start;
		getload(info->avg, AVG_LEN);
		sleep(1);
	}
	
	return EXIT_SUCCESS;
}




