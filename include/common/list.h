#ifndef LIST_SOCKET_H
#define LIST_SOCKET_H

typedef struct node node_t;

node_t*
create_node(void *val);

void
add_node(node_t **head, void *val);

int
match_node(node_t *node, int key);

void
delete_list(node_t **head);

void
print_list(node_t *head, void print_node(void *));

node_t*
find_node(node_t *head, int match_node(node_t *node, int), int find_key);

#endif
