#include <stdio.h> // for puts
#include <stdlib.h> // for EXIT_SUCCESS

#include "mazingerz/server.h"
#include "common/socket.h" // for getsockname_for

#define SV_SOCK_PATH "/tmp/mazingerz.socket"

int
start_server(serverconf_t *serverconf)
{
        int sfd = create_socket(SV_SOCK_PATH);
        set_receive_timeout_socket(sfd);

        serverconf->sfd = sfd;

        return EXIT_SUCCESS;
}

int
stop_server(serverconf_t *serverconf)
{
        printf("Server exiting gracefully\n");

        remove_socket(serverconf->sfd);

        // TODO: wait cleanup all threads

        return EXIT_SUCCESS;
}

#ifdef TEST
#include <string.h> // for strcmp
#include <unistd.h> // for access, F_OK

#include "common/test.h"
#include "sys/socket.h"

void
test_start_server()
{
        serverconf_t serverconf;

        start_server(&serverconf);

        char socket_name[50];
        get_socket_name(serverconf.sfd, socket_name);

        int file_exists = 0;

        if (access(SV_SOCK_PATH, F_OK) != -1)
                file_exists = 1;

        assert("server socket expected name", strcmp(socket_name, SV_SOCK_PATH) == 0);
        assert("server socket exists", file_exists == 1);
}

#endif
