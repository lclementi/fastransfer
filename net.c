/* copyright (c) 2014 Matilda Helou <heinousbutch@gmail.com> */
#include "net.h"

ssize_t do_splice(int fd_in, int fd_out, int pipefd[2], size_t count) {
    ssize_t bytes, bytes_sent, bytes_in_pipe;
    size_t total_bytes_sent = 0;

    // Splice the data from fd_in into the pipe
    while (total_bytes_sent < count) {
        if ((bytes_sent = splice(fd_in, NULL, pipefd[1], NULL,
                count - total_bytes_sent, 
                SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
            if (bytes_sent == 0) {
                return 0;
            }

            if (errno == EINTR || errno == EAGAIN) {
                // Interrupted system call/try again
                // Just skip to the top of the loop and try again
                continue;
            }
            perror("splice");
            return -1;
        }

        // Splice the data from the pipe into fd_out
        bytes_in_pipe = bytes_sent;
        while (bytes_in_pipe > 0) {
            if ((bytes = splice(pipefd[0], NULL, fd_out, NULL, bytes_in_pipe,
                    SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
                if (errno == EINTR || errno == EAGAIN) {
                    // Interrupted system call/try again
                    // Just skip to the top of the loop and try again
                    continue;
                }
                perror("splice");
                return -1;
            }
            bytes_in_pipe -= bytes;
        }
        total_bytes_sent += bytes_sent;
	if (total_bytes_sent != count)
		printf("splicing multiple times (%zu)\n", count - total_bytes_sent);
    }
    return total_bytes_sent;
}
