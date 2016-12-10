#include <stdio.h>  // for perror
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // for strcpy, strcmp
#include <sys/un.h> // for sockaddr_un

#include "mazingerz/server.h"

client_t*
new_client(const char *address)
{
        ssize_t address_len = strlen(address) + 1;

        client_t *nclient = malloc(sizeof(client_t));
        nclient->address = calloc(address_len, sizeof(char));
        if (strcpy(nclient->address, address) != 0) {
                perror("add_client#strcp");
                return NULL;
        }
        return nclient;
}

void
register_client(serverconf_t *serverconf, struct sockaddr_un claddr)
{
        printf("Received message from %s\n", claddr.sun_path);
        client_t *client;
        if ((client = new_client(claddr.sun_path))) {
                list_add(&client->entry, &serverconf->list_of_clients);
        }
}

client_t*
find_client_by_address(serverconf_t *serverconf, const char *address)
{
        client_t *found;
        client_t *client;
        list_for_each_entry(client, &serverconf->list_of_clients, entry) {
                if (strcmp(client->address, address) == 0) {
                        found = client;
                }
        }
        return found;
}
