#ifndef COMMON_LIST_H
#define COMMON_LIST_H

#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

struct node {
        void *val;
        struct node *next;
};

struct node*
new_node(void *val)
{
        struct node *node = malloc(sizeof(struct node));
        if (node != NULL) {
                node->val = val;
                node->next = NULL;
        }
        return node;
}

int
add_node(struct node **head, void *val)
{
        struct node *node = malloc(sizeof(struct node));
        if (node != NULL) {
                node->val = val;
                node->next = *head;
                *head = node;
                return 1;
        }
        return 0;
}


void
print_list(struct node *head, void print_node(void *))
{
        struct node *current = head;
        while(current != NULL) {
                print_node(current->val);
                current = current->next;
        }
}

void
delete_list(struct node **head)
{
        while(*head != NULL) {
                struct node *next_node = (*head)->next;
                free((*head)->val);
                free(*head);
                *head = next_node;
        }
}

struct node*
find_node(struct node *head, int match_node(void *, void *), void *value)
{
        struct node *current = head;
        while(current != NULL) {
                if (match_node(current->val, value) == 1)
                        break;
                current = current->next;
        }
        return current;
}
#endif

#ifdef TEST
#include "common/test.h"
// Helpers

int
match_node(void *nodeval, void *searchval)
{
        if (*(int *)nodeval == *(int *)searchval)
                return 1;
        else
                return 0;
}

// Tests

void
test_add_node()
{
        struct node *head = NULL;

        int *value = malloc(sizeof(int));
        *value = 5;
        int ret = add_node(&head, (void *)value);
        assert("it adds a node on empty list",
                        (ret == 1) && (*(int *)head->val == 5));

        value = malloc(sizeof(int));
        *value = 3;
        ret = add_node(&head, (void *)value);
        assert("it adds a node to not empty list",
                        (ret == 1) &&
                        (*(int *)head->val == 3) &&
                        (*(int *)head->next->val == 5));

        delete_list(&head);
}

void
test_find_node()
{
        struct node *head = NULL;
        int *value1 = malloc(sizeof(int));
        *value1 = 5;
        add_node(&head, value1);
        int *value2 = malloc(sizeof(int));
        *value2 = 3;
        add_node(&head, value2);

        int present = 5;
        struct node *searchnode = find_node(head, match_node, &present);
        assert("it returns the node when it is present",
                        *(int *)searchnode->val == present);

        int notpresent = 6;
        searchnode = find_node(head, match_node, &notpresent);
        assert("it returns NULL when the node is not present",
                        searchnode == NULL);

        delete_list(&head);
}

int
main()
{
        setup_test_runner();
        test_add_node();
        test_find_node();
}
#endif
