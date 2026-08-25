#include "net_helper.h"

#include <netdb.h>

#include <unistd.h>
#include <sys/socket.h>

#include <string.h>

int net_socket_create(int* const restrict sock, const int type) {
  if (*sock >= 0)
    return 1;

  if ((*sock = socket(AF_INET, type, 0)) < 0) {
    return 1;
  }

  return 0;
}

int net_socket_close(int* const restrict sock) {
  if (*sock >= 0) {
    if (close(*sock)) // Socket already closed or never assigned
      return 1;
  }
  
  *sock = -1;

  return 0;
}

/** ----------------------------------------------------------- *
  *  resolve_domain                                             *
  *                                                             *
  *  Convert domain name server into public IP.                 *
  *                                                             *
  *  @param hostname  [in] Domain name server.                  *
  *  @param out_addr [out] Public IP address.                   *
  *                                                             *
  *  @retval 0 Host conversion successful.                      *
  *  @retval 1 Host comversion failed.                          *
  * ----------------------------------------------------------- **/

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
