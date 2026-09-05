#ifndef NET_ADDR_H
#define NET_ADDR_H

#define _POSIX_C_SOURCE 200809L

#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>

#else
#include <netinet/in.h>
#endif

int net_addr_resolve(const char* hostname, struct sockaddr_in *out_addr);
#endif
