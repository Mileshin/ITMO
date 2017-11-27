#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "extern.h"

int main(){
	msg_t msg;

	int msg_id;

	key_t key = KEY; /* icp key */

	/* setup message queue */
	msg_id = msgget(key, IPC_CREAT | 0666);
	if (msg_id < 0) {
		perror("msgget");
		return EXIT_FAILURE;
	}

	/* get message */
	if (msgrcv(msg_id, &msg, sizeof(msg), 0, 0) < 0) {
		perror("msgrcv");
		return EXIT_FAILURE;
	}

	info_t *info = &(msg.info);
	
	/* print id */
	printf("%ld\n%ld\n%ld\n", (long) info->pid, (long) info->uid, 
			(long) info->gid);
	/* print time */
	printf("%ld\n", (long) info->diff);
	/* print loadavg */
	printf("%.6f %.6f %.6f\n", info->avg[0], 
			info->avg[1], info->avg[2]);

	return EXIT_SUCCESS;
}
