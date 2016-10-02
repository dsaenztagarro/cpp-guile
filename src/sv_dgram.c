#include <signal.h>     // for signal
#include <stdio.h>
#include <stdlib.h>     // for EXIT_SUCCESS
#include <string.h>     // for memset
#include <sys/socket.h> // for recvfrom
#include <sys/un.h>     // for sockaddr_un

#include "common.h"

#define SV_SOCK_PATH "/tmp/udp.server.socket"
#define BUF_SIZE 500

static int exitGracefully = 0;

static void
sigHandler(int sig)
{
        exitGracefully = 1;
}

int
main()
{
        if (signal(SIGINT, sigHandler) == SIG_ERR) {
                errExit("signal");
        }

        struct sockaddr_un claddr;
        int sfd;
        ssize_t numBytes;
        socklen_t len;
        char buf[BUF_SIZE];

        sfd = make_named_socket(SV_SOCK_PATH);

        for(;;) {
                len = sizeof(struct sockaddr_un);
                numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                                (struct sockaddr_un *)&claddr, &len);

                if (numBytes == -1) {
                        errExit("recvfrom");
                }

                printf("Server received %ld bytes from %s\n", (long) numBytes,
                        claddr.sun_path);

                printf("Server received message %s\n", buf);

                if (exitGracefully == 1) {
                        printf("Server exiting gracefully\n");
                        remove_socket(sfd);
                        break;
                }
        }

        exit(EXIT_SUCCESS);
}
