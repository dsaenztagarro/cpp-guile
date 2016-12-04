#include <stdio.h>  // for perror
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // for strcpy, strcmp
#include <sys/un.h> // for sockaddr_un

#include "common/list.h"

node_t *clients;

typedef struct client {
        char *address;     // socket unix path
        pthread_t *thread; // server thread for client
} client_t;

client_t
new_client(const char *address)
{
        client_t *nclient = malloc(sizeof(client_t));
        nclient->address = calloc(address, sizeof(char));
        if (strcpy(nclient->address, address) != 0) {
                perror("add_client#strcp");
                return EXIT_FAILURE;
        }
        return nclient;
}

node_t
new_client_node(

int
add_client(node_t **client, struct sockaddr_un claddr)
{
        client_t *new_client = malloc(sizeof(client_t));
        new_client->address = calloc(claddr.sun_len, sizeof(char));
        if (strcpy(new_client->address, claddr.sun_path) != 0) {
                perror("add_client#strcp");
                return EXIT_FAILURE;
        }

        add_node(client, new_client);
        return EXIT_SUCCESS;
}

int
match_client(void *val, void *search)
{
        return strcmp(((client_t *)val)->address, (char *)search) == 0;
}

node_t*
find_client(node_t *client, struct sockaddr_un claddr)
{
        return find_node(client, match_client, claddr.sun_len);
}
