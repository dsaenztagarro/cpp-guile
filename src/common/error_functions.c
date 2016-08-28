#include <stdio.h>  // for vsnprintf
#include <stdarg.h> // for va_list
#include <stdlib.h> // for exit

#define BUF_SIZE 500

void
errExit(const char *format,...)
{
        va_list ap;

        char user_msg[BUF_SIZE];

        vsnprintf(user_msg, BUF_SIZE, format, ap);
        exit(-1);
}
