#ifndef HOST_H
#define HOST_H

#define _POSIX_C_SOURCE 200809L 

#include <stdint.h>

#include <arpa/inet.h>

int resolve_domain(const char* const restrict hostname, struct sockaddr_in* out_addr);

#endif
