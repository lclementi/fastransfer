

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {
    int pipefd[2];
    int result;
    FILE *in_file;
    FILE *out_file;

    result = pipe(pipefd);

    in_file = fopen(argv[1], "rb");
    out_file = fopen(argv[2], "wb");

    result = splice(fileno(in_file), 0, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    printf("%d\n", result);

    result = splice(pipefd[0], NULL, fileno(out_file), 0, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    printf("%d\n", result);

    if (result == -1)
        printf("%d - %s\n", errno, strerror(errno));

    close(pipefd[0]);
    close(pipefd[1]);
    fclose(in_file);
    fclose(out_file);

    return 0;
}
