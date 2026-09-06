#include "peer_state.h"

#include "logger/logger.h"

#include "net/addr.h"
#include "net/stun_client.h"
#include "net/udp_socket.h"

#include "crypto/random.h"

int peer_context_create(peer_context_t *const restrict ctx)
{
  ctx->state = PEER_UNDEFINED;
  ctx->sock = -1; // Invalid fd 
  
  if (rnds64(&ctx->random))
    return 1;

  if (udp_socket_create(&ctx->sock)) {
    LOG_DEBUG("udp_socket_create() failed.");
    return 1;
  }
  
  return 0;
}

int peer_context_reset_random(peer_context_t *const restrict ctx)
{
  return (rnds64(&ctx->random));
}

int peer_context_check_net_state(peer_context_t *const restrict ctx)
{
  return stun_client_check(&ctx->my_pub_ip);
}

int peer_context_set_port(peer_context_t *const restrict ctx, char* const restrict lport)
{ 
  return udp_socket_open_port(ctx->sock, lport, &ctx->my_port);
}

int peer_context_set_remote(peer_context_t *const restrict ctx, const char* const restrict rip, const char* const restrict rport)
{
  if (net_addr_parse_addr(rip, rport, &ctx->peer_addr))
    return 1;
  
  ctx->state = PEER_ADDED;

  return 0;
}

int peer_context_close(peer_context_t *const restrict ctx)
{
  return udp_socket_close(&ctx->sock);
}
