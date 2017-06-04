#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "extern.h"

int main(){
	info_t *info;
	int mem_id;
	key_t key = KEY; /* ipc key */
	
	/* setup message queue */
	int mem_id = shmget(key, sizeof(info), 0);
	
	if (mem_id < 0){
		perror("shmget");
		return EXIT_FAILURE;
	}

	/* attach segment */
	info = (info_t*) shmat(mem_id, NULL, 0);	
	if (info == NULL) {
		perror("shmat");
		return EXIT_FAILURE;
	}
	
	/* print id */
	printf("%ld\n%ld\n%ld\n", (long) info->pid, (long) info->uid,
			(long) i->gid);
	/* print time */
	printf("%ld\n", (long) info->diff);
	/* print loadavg */
	printf("%.6f %.6f %.6f\n", info->avg[0], 
			info->avg[1], info->avg[2]);
	
	return EXIT_SUCCESS;
}
