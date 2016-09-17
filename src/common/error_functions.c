#include <stdio.h>  // for snprintf, vsnprintf
#include <string.h> // for strerror
#include <stdarg.h> // for va_list
#include <stdlib.h> // for exit
#include <errno.h>  // for errno

#define BUF_SIZE 500

static char *ename[] = { "" };
#define MAX_ENAME 106

void
outputError(int err, const char *format, va_list argList)
{
        char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

        vsnprintf(userMsg, BUF_SIZE, format, argList);

        snprintf(errText, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));

        snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

        fputs(buf, stderr);
        fflush(stderr);
}

void
errExit(const char *format, ...)
{
        va_list argList;

        va_start(argList, format);
        outputError(errno, format, argList);
        va_end(argList);

        exit(-1);
}
