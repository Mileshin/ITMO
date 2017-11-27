#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "extern.h"

int main(){
	int fd;
	struct sockaddr_un addr;
	
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, name);

	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("connect");
		return EXIT_FAILURE;
	}
	
  	info_t *info = (info_t*) malloc(sizeof(info_t));
  	if (!info) {
  		printf("Out of memory.\n");
  		return EXIT_FAILURE;
  	}

	if (read(fd, info, sizeof(info_t)) != sizeof(info_t)) {
		printf("Read failed.\n");
		return EXIT_FAILURE;
	}
	
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
