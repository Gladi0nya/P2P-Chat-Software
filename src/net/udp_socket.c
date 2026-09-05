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

int udp_socket_init_ctx(peer_context_t *const restrict ctx)
{
  ctx->sock      = -1;
  ctx->my_port   =  0;
  ctx->my_pub_ip =  0;
  ctx->peer_addr = (struct sockaddr_in){0};
  ctx->state     = PEER_ADDED;
  ctx->packet_id = 0;
  
  return 0;
}

int udp_socket_open_port_ctx(peer_context_t *const restrict ctx,
			     const char *const restrict my_port)
{
  if (udp_socket_create(&ctx->sock)) {
    LOG_DEBUG("udp_socket_create() failed.");
    return 1;
  }

  ctx->my_port = (uint16_t)atoi(my_port);

  if (stun_client_bind_sock(&ctx->sock, &ctx->my_port)) {
    LOG_DEBUG("stun_client_check() failed.");
    return 1;
  }

  LOG_DEBUG("Socket bound on port %u.", ctx->my_port);

  return 0;
}

int udp_socket_set_peer_ctx(peer_context_t *const restrict ctx,
			    const char *const restrict peer_ip,
			    const char *const restrict peer_port)
{

  if (inet_pton(AF_INET, peer_ip, &ctx->peer_addr.sin_addr.s_addr) != 1) {
    LOG_ERROR("Failed to parse IP.");
    LOG_DEBUG("IP: %s", peer_ip);
    return 1;
  }

  ctx->peer_addr.sin_family = AF_INET;
  ctx->peer_addr.sin_port   = htons((uint16_t)atoi(peer_port));

  return 0;
}

int udp_socket_close_ctx(peer_context_t *const restrict ctx)
{
  return udp_socket_close(&ctx->sock);
}
