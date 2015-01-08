/* copyright (c) 2013 Matilda Helou <heinousbutch@gmail.com> */

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define SPLICE_SIZE  65536 //default pipe duffer size in linux

//8192 //671862


ssize_t send_all(int socket, const uint8_t *buffer, size_t length, int flag);
ssize_t recv_all(int socket, uint8_t *buffer, size_t length, int flag);
ssize_t write_all(int fd, const uint8_t *buffer, size_t count);
ssize_t do_splice(int fd_in, int fd_out, int pipefd[2], size_t count);
