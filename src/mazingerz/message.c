#include <stdio.h>         // for sscanf, printf, fflush
#include <string.h>        // for strlen
#include <stdlib.h>        // for malloc, free

#include <sys/syslimits.h> // for PATH_MAX (macos)
// #include <linux/limits.h> // for PATH_MAX (ubuntu)

#include "common/list.h"

/* Message format:
 *
 * WatchableDir
 * WatchableType1 WatchablePattern1
 * WatchableType2 WatchablePattern2
 */


// struct sockaddr_un socket; // client socket
// pthread_t thread; // server thread
// char active;

typedef struct message {
        char basedir[200]; // PATH_MAX
        node_t *watchables;
} message_t;

typedef struct watchable {
        char *type;
        char *pattern;
} watchable_t;


int
extract_message(message_t **message, char input[])
{
        *message = malloc(sizeof(message_t));

        int bytes_read, total_bytes_read;
        bytes_read = total_bytes_read = 0;

        char type[50];
        char pattern[50];

        if (sscanf(input, "%s\n%n", (*message)->basedir, &bytes_read) == 1)
                total_bytes_read += bytes_read;
        else
                return -1;

        printf("READ basedir: %s\n", (*message)->basedir);

        while (sscanf(input + total_bytes_read, "%s %s\n%n", type, pattern, &bytes_read) == 2) {
                printf("READ %s %s\n", type, pattern);
                total_bytes_read += bytes_read;
        }

        fflush(stdout);
        return 1;
}

watchable_t*
create_watchable(const char *type, const char *pattern)
{
        ssize_t type_len = strlen(type) + 1;
        ssize_t pattern_len = strlen(pattern) + 1;

        watchable_t *watchable = malloc(sizeof(watchable_t));
        watchable->type = calloc(type_len, sizeof(char));
        watchable->pattern = calloc(pattern_len, sizeof(char));

        strcpy(watchable->type, type);
        strcpy(watchable->pattern, pattern);

        return watchable;
}

void
free_watchable(watchable_t *ptr_watchable)
{
        free(ptr_watchable->type);
        free(ptr_watchable->pattern);
        free(ptr_watchable);
}

#ifdef TEST
#include "common/test.h"

void
test_extract_message()
{
        char *message = "/tmp/dir\n\
rspec_models spec/models/*.rb\n\
rspec_acceptance spec/acceptance/*.rb";

        // TODO: test with real message

        message_t *ptr_message;

        int ret = extract_message(&message);

        // assert((strcmp(ptr_message->command, "ADD") == 0), "command is parsed");
        // assert((strcmp(ptr_message->data, "XXXXX") == 0), "data is parsed");
        // free_message(ptr_message);
}

int
main()
{
        setup_test_runner();
        test_extract_message();
}
#endif
