#include <stdio.h>
#include <string.h>     // for memset
#include <sys/un.h>     // for sockaddr_un
#include <sys/socket.h> // for socket
#include <unistd.h>     // for getpid

#include "error_functions.h"

#define SV_SOCK_PATH "/tmp/sv_socket_dgram"
#define BUF_SIZE 1024

int
main(int argc, char *argv[])
{
        struct sockaddr_un svaddr, claddr;
        int sfd;
        size_t msgLen;
        ssize_t numBytes;
        char buf[BUF_SIZE];

        if (argc < 2 || strcmp(argv[1], "--help") == 0) {
                errExit("%s msg...\n", argv[0]);
        }

        sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sfd == -1) {
                errExit("socket");
        }

        memset(&claddr, 0, sizeof(struct sockaddr_un));
        claddr.sun_family = AF_UNIX;
        snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/cl_dgram.%d", (long) getpid());

        if (bind(sfd, (struct sockaddr_un *) &claddr, sizeof(struct sockaddr_un)) == -1) {
                errExit("bind");
        }

        memset(&svaddr, 0, sizeof(struct sockaddr_un));
        svaddr.sun_family = AF_UNIX;
        strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

        for (int j = 1; j < argc; j++) {
                msgLen = strlen(argv[j]);
                if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr_un *) &svaddr,
                        sizeof(struct sockaddr_un)) != msgLen) {
                        errExit("sendto");
                }
        }
}
