#include "addr.h"

#include "logger/logger.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>

#else
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <stdlib.h>
#include <string.h>

int net_addr_resolve(const char* const restrict hostname, struct sockaddr_in* out_addr) {
  struct addrinfo hints = {0};
  struct addrinfo* result;
  
  hints.ai_family   = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(hostname, NULL, &hints, &result) != 0) return 1;

  memcpy(out_addr, result->ai_addr, sizeof(struct sockaddr_in));
  
  freeaddrinfo(result);
  
  return 0;
}

int net_addr_parse_addr(const char *const restrict ip,
			const char *const restrict port,
		        struct sockaddr_in *const restrict out_addr)
{
  if (inet_pton(AF_INET, ip, &out_addr->sin_addr.s_addr) != 1) {
    LOG_ERROR("Failed to parse IP.");
    LOG_DEBUG("IP: %s", ip);
    return 1;
  }

  out_addr->sin_family = AF_INET;
  out_addr->sin_port   = htons((uint16_t)atoi(port));

  return 0;
}
