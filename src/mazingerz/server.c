#include <stdio.h> // for puts
#include <stdlib.h> // for EXIT_SUCCESS

#include "mazingerz/server.h"

int
start_server(serverconf_t serverconf)
{
        return EXIT_SUCCESS;
}

int
stop_server(serverconf_t serverconf)
{
        return EXIT_SUCCESS;
}

#ifdef TEST
#include "common/test.h"

void
test_start_server()
{
        serverconf_t serverconf;

        // start_server(serverconf);

        puts("test start server!");
}

#endif
