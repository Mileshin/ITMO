#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/loadavg.h>

#include "extern.h"

info_t	info;

info_t get_info(){
	info_t info;

	info.pid = getpid();
	info.uid = getuid();
	info.gid = getgid();

	info.diff = 0;

	getloadavg(info.avg, 3);

	return info;
}

int main(){
	time_t start = time(NULL);
	time_t cur_time;
	struct sockaddr_un addr;
	int fd, cl, flags;

	info = get_info();

	/* remove socket file if exists */
	unlink(name);
	
	/* open socket descriptor*/
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	/* setup */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, name);
	
	/* bind */
	if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("bind");
		return EXIT_FAILURE;
	}

	flags = fcntl(fd, F_GETFL, 0); /* non-blocking */
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	if (listen(fd, 5) == -1) {
		perror("listen");
		return EXIT_FAILURE;
	}

	while (1) {
		cur_time = time(NULL);

		info.diff = cur_time - start;
		getloadavg(info.avg, 3);
		
		if ((cl = accept(fd, NULL, NULL)) != -1)
			write(cl, &info, sizeof(info));
		sleep(1);
	}

	return EXIT_SUCCESS;
}
