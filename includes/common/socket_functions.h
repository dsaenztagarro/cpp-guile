#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

int
make_named_socket(const char *filename);

int
remove_socket(int sfd);

#endif
