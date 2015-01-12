/* copyright (c) 2014 Matilda Helou <heinousbutch@gmail.com> */
#include "net.h"

#define SPLICE_FLAGS  SPLICE_F_MOVE | SPLICE_F_MORE | SPLICE_F_NONBLOCK

#define SPLICE_SIZE  65536 //default pipe duffer size in linux

//8192 //671862


ssize_t do_splice(int fd_in, int fd_out, int pipefd[2]) {
	size_t total_bytes_sent = 0, nr, ret;

	for (;;) {
		nr = splice(fd_in, NULL, pipefd[1], NULL, SPLICE_SIZE, SPLICE_FLAGS);
		if (nr <= 0)
			break;
#ifdef DEBUG
		printf("read %zu\n", nr);
		fflush(stdout);
#endif
		total_bytes_sent += nr;
		do {
			ret = splice(pipefd[0], NULL, fd_out, NULL, nr, SPLICE_FLAGS);
			if (ret <= 0) {
				perror("couldn't write bytes");
				break;
			}
#ifdef DEBUG
			printf("write %zu\n", ret);
			fflush(stdout);
#endif

			nr -= ret;
		} while (nr);

	}

	return total_bytes_sent;
}
