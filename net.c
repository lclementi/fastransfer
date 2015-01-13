/* copyright (c) 2014 Matilda Helou <heinousbutch@gmail.com> */
#include "net.h"

//#define _GNU_SOURCE
#include <fcntl.h>

#define SPLICE_FLAGS  SPLICE_F_MOVE | SPLICE_F_MORE | SPLICE_F_NONBLOCK

#define SPLICE_SIZE  671862 //default pipe duffer size in linux

#define PIPE_SIZE 1048576

//8192 //671862

size_t splice_size = SPLICE_SIZE;

void set_pipeopt(int pipefd[2]){
#ifdef F_SETPIPE_SZ
	/*
	 *  increasing pipe buffer size
	 *  This is supported only by kernel 2.6.35 or greater
	 *  which means no RHEL 6.X
	 */
	size_t pipe_sz = fcntl(pipefd[0], F_SETPIPE_SZ, PIPE_SIZE);
	if (!pipe_sz)
		perror("setting pipe buffer size");
	pipe_sz = fcntl(pipefd[0], F_GETPIPE_SZ);
	printf("Pipe buffer size is: %zu\n", pipe_sz);
	//splice_size = pipe_sz;
#else
	(void)pipefd;
#endif
}

ssize_t do_splice(int fd_in, int fd_out, int pipefd[2]) {
	size_t total_bytes_sent = 0, nr, ret, initial_pull;

	for (;;) {
		nr = splice(fd_in, NULL, pipefd[1], NULL, SPLICE_SIZE, SPLICE_FLAGS);
		if (nr <= 0)
			break;
#ifdef DEBUG
		printf("read %zu\n", nr);
		fflush(stdout);
#endif
		total_bytes_sent += nr;
		initial_pull = nr;
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
			if (nr)
				printf("multiple push initial pull size %zu remaining %zu", initial_pull, nr);
		} while (nr);

	}

	return total_bytes_sent;
}
