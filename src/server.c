#include <stdio.h>
#include "error_functions.h"

int
main()
{
	errExit("Error: %s", "Server");
	printf("Server");
}
