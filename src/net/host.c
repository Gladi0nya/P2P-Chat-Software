#include "host.h"

#include <stddef.h>

#include <arpa/inet.h>
#include <netdb.h>

int resolve_domain(const char* const restrict hostname, ipv4_t* host_ip) {
  struct addrinfo hints = {0};
  struct addrinfo* result;
  struct sockaddr_in* addr;
  
  hints.ai_family   = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(hostname, NULL, &hints, &result) != 0) return 1;

  addr = (struct sockaddr_in*)result->ai_addr;

  if (inet_ntop(AF_INET, &addr->sin_addr, *host_ip, sizeof(*host_ip)) == NULL)
  {
    freeaddrinfo(result);
    return 1;
  }
  
  freeaddrinfo(result);
  
  return 0;
}
