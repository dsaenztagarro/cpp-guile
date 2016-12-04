#include <errno.h>      // for errno, EAGAIN
#include <pthread.h>    // for pthread_create
#include <stdio.h>
#include <sys/socket.h> // for recvfrom
#include <sys/un.h>     // for sockaddr_un
#include <sys/types.h>

#include "common/error.h"
#include "common/socket.h"
#include "common/macros.h"
#include "mazingerz/signals.c"

#include "mazingerz/event.c"
#include "mazingerz/message.c"
#include "mazingerz/client.c"

#define SV_SOCK_PATH "/tmp/mazingerz.socket"

#define BUF_SIZE 500

void*
fsevent_handler(void * arg);

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
process_message(message_t *message, node_t **threads)
{
        pthread_t *thread;

        if (pthread_create(&thread, NULL, fsevent_handler, message) != 0)
                errExit("pthread_create");

        add_node(*threads, thread);

        free(message);
}

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

int
main()
{
        node_t *threads;

        catch_signals();

        struct sockaddr_un claddr;
        socklen_t len = sizeof(struct sockaddr_un);
        ssize_t num_bytes;
        char buf[BUF_SIZE];

        int sfd = create_socket(SV_SOCK_PATH);
        set_receive_timeout_socket(sfd);

        while(continue_execution()) {
                num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                        (struct sockaddr *)&claddr, &len);

                if (unread_message(num_bytes) == 0) {
                        printf("Received message from %s\n", claddr.sun_path);

                        message_t *message;
                        if (extract_message(&message, buf) == 0)
                                process_message(message, &threads);
                }

        }

        // TODO: iterate with pthread_join

        printf("Server exiting gracefully\n");
        remove_socket(sfd);

        exit(EXIT_SUCCESS);
}

void*
fsevent_handler(void * arg)
{
        message_t *message = (message_t *)arg;
        printf("thread.basedir: %s", message->basedir);

        int i = 0;
        while(continue_execution() && i < 3) {
                printf("Fsevent loop.. %d", i);
                sleep(5);
                i++;
        }
        printf("Thread exiting gracefully\n");

        return NULL;
}
