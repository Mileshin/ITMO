#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 128

int main(int argc, char **argv){
	int fd[2];

	if (argc != 2) {
		fprintf(stderr, "%s: Usage: %s file\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	if (pipe(fd) < 0) {
		perror("pipe");
		return EXIT_FAILURE;
	}

	switch (fork()) {
		case -1:
			perror("fork");
			return EXIT_FAILURE;
		case 0:
			close(fd[0]);
			dup2(fd[1], STDIN_FILENO);
			execl("/usr/bin/wc", "wc", (char*)0);
		default:
			close(fd[1]);
			
			int file = open(argv[1], O_RDONLY);
			if (file < 0) {
				perror(argv[1]);
				return EXIT_FAILURE;
			}

			char buf[2 * BUFFER_SIZE];
			char out[BUFFER_SIZE];

			ssize_t buf_r = 0, j;
			int i, flag = 0;
			while ((buf_r = read(file, buf, BUFFER_SIZE * 2)) > 0) {
				for (i = flag, j = 0; i < buf_r; i += 2, j++) {
					out[j] = buf[i];
				}
				write(fd[0], out, j);
				flag = (buf_r % 2 == 0) ? 0 : 1;
			}

			if (buf_r < 0) {
				perror("read");
				return EXIT_FAILURE;
			}

			close(file);
			close(fd[0]);
			wait(NULL);
	}

	return EXIT_SUCCESS;
}
