/* copyright (c) 2014 Matilda Helou <heinousbutch@gmail.com> */
/* this file is where we have the functions that handle receiving */

#include "net.h"

int make_listener(char *port)
{
    int fd, rval, status;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* let's do fun stuff with getaddrinfo now */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; /* either ipv4 or ipv6, we don't care */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;


    rval = getaddrinfo(NULL, port, &hints, &result);
    if (rval != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rval));
        exit(1);
    }
    /* now we iterate over the lists of results that getaddrinfo returned
       until we can successfully make a socket and connect with it */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1)
        {
            /* the socket making failed, so we need to do a different 
               address */
            continue;
        }

        /* we made a socket, now we try to bind  */
        if (bind(fd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            break;  /* we successfully bound, let's exit this loop */
        }

        close(fd); /* making the socket worked but bind() failed so we 
                      close this socket */
    }
    if (rp == NULL) /* no address worked */
    {
        fprintf(stderr, "Could not bind to %s\n", port);
        exit(1);
    }
    freeaddrinfo(result);
    listen(fd, 1);
    status = accept(fd, NULL, NULL);
    if(status == -1)
    {
        perror("accept");
        exit (-1);
    }

    
    return status;
}



int main(int argc, char* argv[])
{
    int infd, outfd;
    int pipefd[2];
    ssize_t rval;

    if (argc != 3) {
        printf("usage: %s filename port\n", argv[0]);
        exit(10);
    }

    outfd = open(argv[1], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    
    infd = make_listener(argv[2]);
    
    if (pipe(pipefd) != 0) {
        perror("pipe");
        exit(1);
    }

    set_pipeopt(pipefd);

    while (1) {
        rval = do_splice(infd, outfd, pipefd);
        
        if (rval == -1) {
            perror("splice");
            exit(1);
        }

        if (rval == 0) {
            break;
        }
    }

    close(infd);
    close(outfd);

    return 0;
}

