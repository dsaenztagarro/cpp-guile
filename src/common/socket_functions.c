#include <string.h>     // for memset
#include <sys/socket.h> // for socket, bind
#include <sys/un.h>     // for sockaddr_un

int
make_named_socket(const char *socket_path)
{
        int sfd;
        struct sockaddr_un addr;

        sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sfd == -1) {
                errExit("socket");
        }

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

        if (bind(sfd, (struct sockaddr_un *) &addr, sizeof(struct sockaddr_un)) == -1) {
                errExit("bind");
        }

        return sfd;
}

int
remove_socket(int sfd)
{
        struct sockaddr_un addr;

        if (getsockname(sfd, (struct sockaddr_un *) &addr, sizeof(struct sockaddr_un)) != -1) {
                errExit("getsockname");
        }

        remove(addr.sun_path);
}
