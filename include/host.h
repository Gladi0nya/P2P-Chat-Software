#ifndef HOST_H
#define HOST_H

#define NOT_AN_IP "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

#include <stdint.h>

typedef char ipv4_t[16];

int resolve_domain(const char* const restrict hostname, ipv4_t* host_ip);

#endif
