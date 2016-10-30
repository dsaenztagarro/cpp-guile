#include <stdio.h> // for perror
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE

#include "common/list.h"

node_t *clients;

typedef struct client {
        char *address; // socket unix path
} client_t;

int
add_client(node_t **client, struct sockaddr_un claddr)
{
        client_t *new_client = malloc(sizeof(client_t));
        new_client->address = calloc(claddr.sun_len, sizeof(char));
        if (strcp(new_client->address, claddr.sun_path) != 0) {
                perror("add_client#strcp");
                return EXIT_FAILURE;
        }

        add_node(client, new_client);
        return EXIT_SUCCESS;
}


find_client(node_t *client, struct sockaddr_un claddr)
{
        while (client != NULL) {
                if (strcmp(client->address, claddr.sun_len) == 0)
                        return client;
                client = client->next;
        }
        return NULL;
}
