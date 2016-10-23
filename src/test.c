#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc

#include "common/list.h"

void
print_number(void *val)
{
        printf("%d\n", *((int*)val));
}

int
main()
{
        int i = 1;
        int *val = &i;
        node_t *head = create_node(val);

        int var1 = 2;
        int *val1 = &var1;
        add_node(&head, val1);

        int var2 = 3;
        int *val2 = &var2;
        add_node(&head, val2);

        int var3 = 4;
        int *val3 = &var3;
        add_node(&head, val3);

        print_list(head, print_number);
        node_t * search_node = find_node(head, match_node, 3);

        delete_list(&head);
}
