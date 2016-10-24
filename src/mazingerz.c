#include <errno.h>      // for errno, EAGAIN
#include <pthread.h>    // for pthread_create
#include <stdio.h>
#include <stdlib.h>     // for EXIT_SUCCESS
#include <string.h>     // for memset
#include <sys/socket.h> // for recvfrom
#include <sys/un.h>     // for sockaddr_un

#include "common/error.h"
#include "common/socket.h"
#include "common/list.h"
#include "common/macros.h"
#include "mazingerz/signals.c"

#include "mazingerz/event.c"
#include "mazingerz/message.c"

#define SV_SOCK_PATH "/tmp/mazingerz.socket"
#define BUF_SIZE 500

void*
fsevent_handler(void * arg);

void
handle_message(message_t *message)
{
        pthread_t fsevent_thread;

        if (pthread_create(&fsevent_thread, NULL, fsevent_handler, message) > 0)
                errExit("pthread_create");
}



int
main()
{
        catch_signals();

        struct sockaddr_un claddr;
        socklen_t len = sizeof(struct sockaddr_un);
        ssize_t num_bytes;
        char buf[BUF_SIZE];

        int sfd = create_socket(SV_SOCK_PATH);
        set_receive_timeout_socket(sfd);

        while(not_interrupted()) {
                // message_t *message = receive_message(sfd);
                num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                        (struct sockaddr *)&claddr, &len);

                printf("Received message from %s\n", claddr.sun_path);
                printf("Message content: %s\n", buf);

                if (num_bytes >= 0) {
                        message_t *message = extract_message(buf);
                        printf("EXTRACTED %s %s", message->command, message->data);
                } else {
                        if (errno == EAGAIN) {
                                printf("Waiting for messages..\n");
                                fflush(stdout);
                        } else {
                                errExit("recvfrom");
                        }
                }

                handle_message(message);
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
        printf("HANDLER: %s %s", message->command, message->data);

        while(not_interrupted()) {
                printf("Fsevent loop..");
                sleep(5);
        }
        printf("Thread exiting gracefully\n");
        return NULL;
}
