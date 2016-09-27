#include <stdio.h>
#include <string.h>     // for memset
#include <sys/un.h>     // for sockaddr_un
#include <sys/socket.h> // for socket

#include "error_functions.h"

#define SV_SOCK_PATH "/tmp/sv_socket_dgram"
#define BUF_SIZE 500

int
main()
{
        struct sockaddr_un svaddr, claddr;
        int sfd;
        ssize_t numBytes;
        socklen_t len;
        char buf[BUF_SIZE];

        sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sfd == -1) {
                errExit("socket");
        }

        memset(&svaddr, 0, sizeof(struct sockaddr_un));
        svaddr.sun_family = AF_UNIX;
        strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path)-1);

        if (bind(sfd, (struct sockaddr_un *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
                errExit("bind");
        }

        for (;;) {
                len = sizeof(struct sockaddr_un);
                numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                                (struct sockaddr_un *)&claddr, &len);

                if (numBytes == -1) {
                        errExit("bind");
                }

                printf("Server received %ld bytes from %s\n", (long) numBytes,
                        claddr.sun_path);

                printf("Server received message %s", buf);
        }

	printf("Server");
}
