#include <errno.h>      // for errno, EAGAIN
#include <pthread.h>    // for pthread_create
#include <stdio.h>
#include <sys/socket.h> // for recvfrom
#include <sys/un.h>     // for sockaddr_un
#include <sys/types.h>

#include "common/error.h"
#include "common/socket.h"
#include "common/macros.h"

#include "mazingerz/server.h"
#include "mazingerz/signals.h"
#include "mazingerz/message.h"

#define BUF_SIZE 500

void*
fsevent_handler(void * arg)
{
        clientconf_t *conf = (clientconf_t *)arg;
        printf("thread.basedir: %s", conf->basedir);

        int i = 0;
        while(continue_execution() && i < 3) {
                printf("Fsevent loop.. %d", i);
                sleep(5);
                i++;
        }
        printf("Thread exiting gracefully\n");

        return NULL;
}

int
unread_message(int num_bytes)
{
        if (num_bytes > 0) return 0;

        if (errno == EAGAIN) {
                printf("Waiting for messages..\n");
                fflush(stdout);
                return 1;
        } else {
                errExit("recvfrom");
                stop_execution();
                return -1;
        }
}

void
update_clientconf(clientconf_t *clientconf) //, node_t **threads)
{
        printf("{ basedir: \"%s\" }\n", &clientconf->basedir);

        watched_t *watched;
        list_for_each_entry(watched, &clientconf->list_of_watcheds, entry) {
                printf("{ id: \"%s\", pattern: \"%s\" }\n", watched->id, watched->pattern);
        }

        /*
        pthread_t *thread;

        if (pthread_create(&thread, NULL, fsevent_handler, conf) != 0)
                errExit("pthread_create");

        add_node(*threads, thread);

        free(conf);
        */
}

/*
void
wait_for_threads(node_t **head)
{
        while(*head != NULL) {
                struct node *next_node = (*head)->next;
                free((*head)->val);
                free(*head);
                *head = next_node;
        }
}
*/

#ifndef TEST

int
main()
{
        serverconf_t serverconf;

        catch_signals();

        struct sockaddr_un claddr;
        socklen_t len = sizeof(struct sockaddr_un);
        ssize_t num_bytes;
        char buf[BUF_SIZE];

        start_server(&serverconf);

        while(continue_execution()) {
                // TODO:
                num_bytes = recvfrom(serverconf.sfd, buf, BUF_SIZE, 0,
                        (struct sockaddr *)&claddr, &len);

                if (unread_message(num_bytes) == 0) {

                        register_client(&serverconf, claddr);

                        clientconf_t *clientconf;
                        if (extract_message(&clientconf, buf) == 0)
                                update_clientconf(clientconf);
                }

        }

        stop_server(&serverconf);

        exit(EXIT_SUCCESS);
}

#else

#include "common/test.h"

int
main()
{
        setup_test_runner();
        test_start_server();
        test_stop_server();
        test_extract_message();
}

#endif
