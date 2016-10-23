#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H

#define NORETURN __attribute__ ((__noreturn__))

void
errExit(const char *,...);

#endif
