#ifndef NET_ADDR_H
#define NET_ADDR_H

#define _POSIX_C_SOURCE 200809L

#include <stdint.h>

#include <netinet/in.h>

int net_addr_resolve(const char* hostname, struct sockaddr_in *out_addr);
#endif
