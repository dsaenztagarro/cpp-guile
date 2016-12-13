#include <stdio.h>      // for remove
#include <stdlib.h>     // for EXIT_SUCCESS
#include <string.h>     // for memset
#include <sys/socket.h> // for socket, bind, send_to
#include <sys/un.h>     // for sockaddr_un
#include <sys/time.h>   // for timeval

#include "common/error.h"

int
create_socket(const char *socket_path)
{
	int sfd;
	struct sockaddr_un addr;

	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sfd == -1)
		errExit("create_socket#socket");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

	if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
		errExit("create_socket#bind");

	return sfd;
}

int
remove_socket(int sfd)
{
        int len = sizeof(struct sockaddr_un);
	struct sockaddr_un addr;

	if (getsockname(sfd, (struct sockaddr *)&addr, (socklen_t *)&len) != 0)
		errExit("remove_socket#getsockname");

	return remove(addr.sun_path);
}

int
get_socket_name(int sfd, char *socket_name)
{
        int len = sizeof(struct sockaddr_un);
	struct sockaddr_un addr;

	if (getsockname(sfd, (struct sockaddr *)&addr, (socklen_t *)&len) != 0)
		errExit("get_socket_name#getsockname");

        strcpy(socket_name, addr.sun_path);

        return EXIT_SUCCESS;
}

void
set_receive_timeout_socket(int sfd)
{
	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		errExit("set_receive_timeout_socket#setsockopt\n");
}

#ifdef TEST

/*
 * @param sfd [int] The sender socket file descriptor
 * @param socket_path [char*] The destiny socket unix path
 * @param message [char*] The message to be sent
 */
int
send_message_to_socket(int sfd, const char *socket_path, const char *message)
{
        struct sockaddr_un dest_addr;
        memset(&dest_addr, 0, sizeof(struct sockaddr_un));
        dest_addr.sun_family = AF_UNIX;
        strncpy(dest_addr.sun_path, socket_path, sizeof(dest_addr.sun_path) - 1);

        int message_len = strlen(message);
        if (sendto(sfd, message, message_len, 0, (struct sockaddr_un *) &dest_addr,
                sizeof(struct sockaddr_un)) != message_len) {
                errExit("sendto");
        }

}

#endif
