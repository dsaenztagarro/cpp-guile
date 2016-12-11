#ifndef MAZINGERZ_MESSAGE_H
#define MAZINGERZ_MESSAGE_H

typedef struct clientconf {
        char basedir[200]; // PATH_MAX
        struct list_head list_of_watcheds;
} clientconf_t;

typedef struct watched {
        char *id;
        char *pattern;
        struct list_head entry;
} watched_t;

int
extract_message(clientconf_t **message, char input[]);

#ifdef TEST

void
test_extract_message();

#endif

#endif
