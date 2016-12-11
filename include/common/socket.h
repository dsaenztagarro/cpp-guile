#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

int
create_socket(const char *filename);

int
remove_socket(int sfd);

int
set_receive_timeout_socket(int sfd);

void
check_errors_recvfrom(int num_bytes);

#ifdef TEST

int
getsockname_for(int sfd);

#endif

#endif
