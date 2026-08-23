#include "host.h"

#include <stddef.h>
#include <string.h>

#include <netdb.h>

int resolve_domain(const char* const restrict hostname, struct sockaddr_in* out_addr) {
  struct addrinfo hints = {0};
  struct addrinfo* result;
  
  hints.ai_family   = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(hostname, NULL, &hints, &result) != 0) return 1;

  memcpy(out_addr, result->ai_addr, sizeof(struct sockaddr_in));
  
  freeaddrinfo(result);
  
  return 0;
}
