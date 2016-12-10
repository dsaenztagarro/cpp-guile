#ifndef MAZINGERZ_SERVER_H
#define MAZINGERZ_SERVER_H

#include <pthread.h> // for pthread_t

#include "common/list.h"

typedef struct serverconf {
        int sfd; // server socket file descriptor
        struct list_head list_of_clients;
} serverconf_t;

typedef struct client {
        char *address;     // socket unix path
        pthread_t *thread; // server thread for client
        struct list_head entry;
} client_t;

void
test_start_server();

#endif
