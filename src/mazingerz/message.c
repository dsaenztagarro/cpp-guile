#include <stdio.h>         // for sscanf, printf, fflush
#include <string.h>        // for strlen
#include <stdlib.h>        // for malloc, free

#include <sys/syslimits.h> // for PATH_MAX (macos)
// #include <linux/limits.h> // for PATH_MAX (ubuntu)

#include "common/list.h"
#include "mazingerz/message.h"

/* Message format:
 *
 * {"basedir":"zzzzz"}
 * {"id":"xxxx","pattern":"XXXXX"}
 * {"id":"yyyy","pattern":"YYYYY"}
 */

const char COMMON_WATCHED_FORMAT[] = "{\"basedir\":\"%[^\"]\"}\n%n";
const char WATCHED_FORMAT[] = "{\"id\":\"%[^\"]\",\"pattern\":\"%[^\"]\"}\n%n";

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

// TODO: get_clientconf_from_message (rename)
int
extract_message(clientconf_t **message, char input[])
{
        clientconf_t *client = malloc(sizeof(clientconf_t));
        INIT_LIST_HEAD(&client->list_of_watcheds);

        int bytes_read, total_bytes_read;
        bytes_read = total_bytes_read = 0;

        char id[50];
        char pattern[50];

        if (sscanf(input, COMMON_WATCHED_FORMAT, client->basedir, &bytes_read) == 1)
                total_bytes_read += bytes_read;
        else
                return -1;

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

        clientconf_t *message;

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

        assert(list_empty(&message->list_of_watcheds) != 1, "list of watcheds is not empty");
}

#endif
