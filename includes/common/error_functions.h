#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#define NORETURN __attribute__ ((__noreturn__))

void
errExit(const char *format,...);

#endif
