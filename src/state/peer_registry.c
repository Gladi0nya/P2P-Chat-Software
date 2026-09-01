#include "peer_registry.h"

#include "logger/logger.h"

#include <sys/socket.h>
#include <arpa/inet.h>

int peer_registry_add(peer_context_t *ctx)
{
  struct sockaddr_in local;

  socklen_t len     = sizeof(local);
  
  if (getsockname(ctx->sock, (struct sockaddr*)&local, &len) == 0)
    LOG_INFO("Local port: %u", ntohs(local.sin_port));
    
  
  LOG_DEBUG("Hole punching to %u.%u.%u.%u:%u...",
	    (ctx->peer_addr.sin_addr.s_addr      ) & 0xFF,
	    (ctx->peer_addr.sin_addr.s_addr >>  8) & 0xFF,
	    (ctx->peer_addr.sin_addr.s_addr >> 16) & 0xFF,
	    (ctx->peer_addr.sin_addr.s_addr >> 24) & 0xFF,
	    ntohs(ctx->peer_addr.sin_port));

  ctx->state     = PEER_PUNCHING;
  ctx->packet_id = 0;

  return 0;
}
