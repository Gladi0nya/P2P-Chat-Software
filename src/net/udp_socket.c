#include "udp_socket.h"
#include "stun_client.h"

#include "logger/logger.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#else
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#endif

int udp_socket_create(int* const restrict sock)
{
  if (*sock >= 0)
    return 1;

  if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    LOG_DEBUG("socket() failed.");

    return 1;
  }

  return 0;
}

int udp_socket_close(int* const restrict sock)
{
  if (*sock >= 0) {
    #ifdef _WIN32
    if (shutdown(*sock, SD_BOTH))
      return 1;
    if (closesocket(*sock))
      return 1;
    #else
    if (close(*sock)) // Socket already closed or never assigned
      return 1;
    #endif
  }
  
  *sock = -1;

  return 0;
}


int udp_socket_open_port(int sock, const char *const restrict my_port,
			 uint16_t *const restrict out_port)
{
  *out_port = (uint16_t)atoi(my_port);

  if (stun_client_bind_sock(sock, out_port)) {
    LOG_DEBUG("stun_client_check() failed.");
    return 1;
  }

  LOG_DEBUG("Socket bound on port %u.", *out_port);

  return 0;
}
