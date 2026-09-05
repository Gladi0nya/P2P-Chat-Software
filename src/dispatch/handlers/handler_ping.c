#include "net/ping.h"

#include "handler_ping.h"

#include "logger/logger.h"

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#include <endian.h>
#endif

void handle_ping(peer_context_t *ctx, uint8_t *buffer, ssize_t n)
{
  if (n < (ssize_t)sizeof(uint64_t)) {
    LOG_ERROR("Invalid packet size.");
    return;
  }

  #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
  uint64_t id = ntohll(*(uint64_t*)buffer);
  #else
  uint64_t id = *(uint64_t*)buffer;
  #endif
  
  while (send_pong(ctx, id))
    continue;

  ctx->flags |= 0x1; // My turn to send ping

  LOG_DEBUG("Ping received. ID: %llu", id);
}

void handle_pong(peer_context_t *ctx)
{
  ctx->flags &= 0xFE;

  LOG_DEBUG("Pong received.");
}
