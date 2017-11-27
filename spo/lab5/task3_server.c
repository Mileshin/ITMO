#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/loadavg.h>

#include "extern.h"

info_t get_info(){
	info_t info;

	info.pid = getpid();
	info.uid = getuid();
	info.gid = getgid();
	info.diff = 0;

	getloadavg(info.avg, AVG_LEN);

	return info;
}

int main(){
	time_t start = time(NULL);
	time_t cur_time;
	int fd;
	
	/* remove socket file if exixts */
	unlink(name);
	
	/* create ipc file */
	fd = open(name, O_RDWR | O_CREAT, 0644);
	if (fd < 0) {
		perror(name);
		return EXIT_FAILURE;
	}

	/* memory-mapping */ 
	info_t *info = (info_t*) mmap(NULL, sizeof(info_t), PROT_READ | 
									PROT_WRITE, MAP_SHARED, fd, 0);
	if (info == MAP_FAILED) {
		perror("mmap");
		return EXIT_FAILURE;
	}

	/* assure enough space */
	truncate(name, sizeof(info_t));

	*info = get_info();

	while (1) {
		cur_time = time(NULL);

		info->diff = cur_time - start;
		getloadavg(info->avg, AVG_LEN);

		sleep(1);
	}

	return EXIT_SUCCESS;
}
