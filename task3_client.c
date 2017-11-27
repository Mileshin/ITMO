#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "extern.h"

int main(){
	int fd;

	/* open ipc file */
	fd = open(name, O_RDWR);
	if (fd < 0) {
		perror(name);
		return EXIT_FAILURE;
	}

	/* memory-mapping */
	info_t *info = (info_t*) mmap(NULL, sizeof(info_t), PROT_READ, 
							MAP_SHARED, fd, 0);
	if (info == MAP_FAILED) {
		perror("mmap");
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
