#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

struct node {
        void *val;
        struct node *next;
};

struct node*
create_node(void *val)
{
        struct node *node = NULL;
        node = malloc(sizeof(struct node));
        node->val = val;
        node->next = NULL;
        return node;
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
                free(*head);
                *head = next_node;
        }
}

void
add_node(struct node **head, void *val)
{
        struct node *new_node = malloc(sizeof(struct node));
        new_node->val = val;
        new_node->next = *head;

        *head = new_node;
}

int
match_node(struct node *node, int key)
{
        return *(int *)node->val == key;
}

struct node*
find_node(struct node *head, int match_node(struct node *node, int), int find_key)
{
        struct node *current = head;
        while(current != NULL) {
                if (match_node(current, find_key) == 1)
                        break;
                current = current->next;
        }
        return current;
}
