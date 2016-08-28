#include <stdio.h>  // for vsnprintf
#include <stdarg.h> // for va_list
#include <stdlib.h> // for exit

#define BUF_SIZE 500

void
errExit(const char *format, ...)
{
        va_list ap;

        char user_msg[BUF_SIZE];

        va_start(ap, format);
        vsnprintf(user_msg, BUF_SIZE, format, ap);
        printf(user_msg);
        exit(-1);
}
