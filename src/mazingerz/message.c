#include <stdio.h>         // for sscanf, printf, fflush
#include <string.h>        // for strlen
#include <stdlib.h>        // for malloc, free

#include <sys/syslimits.h> // for PATH_MAX (macos)
// #include <linux/limits.h> // for PATH_MAX (ubuntu)

#include "common/list.h"

/* Message format:
 *
 * {"basedir":"zzzzz"}
 * {"id":"xxxx","pattern":"XXXXX"}
 * {"id":"yyyy","pattern":"YYYYY"}
 */

// struct sockaddr_un socket; // client socket
// pthread_t thread; // server thread
// char active;

typedef struct message {
        char basedir[200]; // PATH_MAX
        struct list_head list_of_watcheds;
} message_t;

typedef struct watched {
        char *id;
        char *pattern;
        struct list_head entry;
} watched_t;

watched_t*
create_watched(const char *id, const char *pattern)
{
        ssize_t id_len = strlen(id) + 1;
        ssize_t pattern_len = strlen(pattern) + 1;

        watched_t *watched = malloc(sizeof(watched_t));
        if (!watched) return NULL;
        watched->id = calloc(id_len, sizeof(char));
        if (!watched->id) return NULL;
        watched->pattern = calloc(pattern_len, sizeof(char));
        if (!watched->pattern) return NULL;

        strcpy(watched->id, id);
        strcpy(watched->pattern, pattern);

        return watched;
}

int
extract_message(message_t **message, char input[])
{
        message_t *client = malloc(sizeof(message_t));
        INIT_LIST_HEAD(&client->list_of_watcheds);

        int bytes_read, total_bytes_read;
        bytes_read = total_bytes_read = 0;

        char id[50];
        char pattern[50];

        if (sscanf(input, "{\"basedir\":\"%[^\"]\"}\n%n", client->basedir, &bytes_read) == 1)
                total_bytes_read += bytes_read;
        else
                return -1;

        const char WATCHED_FORMAT[] = "{\"id\":\"%[^\"]\",\"pattern\":\"%[^\"]\"}\n%n";

        while (sscanf(input + total_bytes_read, WATCHED_FORMAT, id, pattern, &bytes_read) == 2) {
                watched_t *watched = create_watched(id, pattern);
                list_add(&watched->entry, &(client->list_of_watcheds));
                total_bytes_read += bytes_read;
        }
        fflush(stdout);

        *message = client;
        return 0;
}

void
free_watched(watched_t *ptr_watched)
{
        free(ptr_watched->id);
        free(ptr_watched->pattern);
        free(ptr_watched);
}

#ifdef TEST
#include "common/test.h"

void
test_extract_message()
{
        char input[] = "{\"basedir\":\"/Users/test/Code/\"}\n\
{\"id\":\"rspec\",\"pattern\":\"spec/models/*.rb\"}\n\
{\"id\":\"rspec_acceptance\",\"pattern\":\"spec/acceptance/*.rb\"}\n";

        // TODO: test with real message

        message_t *message;

        int ret = extract_message(&message, input);

        if (!list_empty(&message->list_of_watcheds)) {
                watched_t *watched;
                list_for_each_entry(watched, &message->list_of_watcheds, entry) {
                        printf("{ id: \"%s\", pattern: \"%s\" }\n", watched->id, watched->pattern);
                }
                puts("List not empty");
        }
        else
                puts("List empty");

        assert((!list_empty(&message->list_of_watcheds) == 0), "list of watcheds is not empty");
}

int
main()
{
        setup_test_runner();
        test_extract_message();
}
#endif
