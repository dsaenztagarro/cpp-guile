#include <stdio.h>  // for puts
#include <stdlib.h> // for EXIT_SUCCESS

#include "common/socket.h"     // for getsockname_for
#include "mazingerz/server.h"
#include "mazingerz/message.h" // for MESSAGE_TEST

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
        assert("server socket file exists", file_exists == 1);

        stop_server(&serverconf);
}

void
test_receive_message()
{
        serverconf_t serverconf;
        start_server(&serverconf);

        int client_sfd = create_socket("/tmp/test_client.socket");
        send_message_to_socket(client_sfd, SV_SOCK_PATH, MESSAGE_TEST);

        stop_server(&serverconf);
}

void
test_stop_server()
{

        serverconf_t serverconf;

        start_server(&serverconf);

        stop_server(&serverconf);

        int file_exists = 1;
        if (access(SV_SOCK_PATH, F_OK) == -1)
                file_exists = 0;

        assert("server socket file does not exist", file_exists == 0);
}

#endif
