#ifndef NET_HELPER_H
#define NET_HELPER_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L 

#include <arpa/inet.h>

int net_socket_create(int* const restrict sock, const int type);
int net_socket_close(int* const restrict sock);
int resolve_domain(const char* const restrict hostname, struct sockaddr_in* out_addr);
#endif
