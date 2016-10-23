#include <stdio.h>  // for sscanf, printf
#include <string.h> // for strlen
#include <stdlib.h> // for malloc, free

typedef struct message {
        char *command;
        char *data;
} message_t;

message_t*
extract_message(char input[])
{
        char command[3];
        char data[10];
        message_t *message = NULL;

        if (sscanf(input, "%s %s", command, data) == 2) {
                ssize_t command_len = strlen(command) + 1;
                ssize_t data_len = strlen(data) + 1;

                message = malloc(sizeof(message_t));
                message->command = malloc(command_len * sizeof(char));
                message->data = malloc(data_len * sizeof(char));

                strcpy(message->command, command);
                strcpy(message->data, data);
        } else {
                printf("Invalid format");
                // return result_error("Invalid format", 2);
        }

        fflush(stdout);
        return message;
}

void
free_message(message_t *ptr_message)
{
        free(ptr_message->command);
        free(ptr_message->data);
        free(ptr_message);
}

#ifdef TEST
#include "common/test.h"

void
test_extract_message()
{
        message_t *ptr_message;

        ptr_message = extract_message("ADD XXXXX");
        assert((strcmp(ptr_message->command, "ADD") == 0), "command is parsed");
        assert((strcmp(ptr_message->data, "XXXXX") == 0), "data is parsed");
        free_message(ptr_message);
}

int
main()
{
        test_extract_message();
}
#endif
