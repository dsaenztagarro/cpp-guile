#include <stdio.h>      // for remove
#include <string.h>     // for memset
#include <sys/socket.h> // for socket, bind
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
		errExit("remove_socket#getsock_name");

	return remove(addr.sun_path);
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
