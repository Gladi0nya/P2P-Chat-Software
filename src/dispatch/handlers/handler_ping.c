#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "net/ping.h"

#include "handler_ping.h"

#include "logger/logger.h"

#include <time.h>

void handle_ping(peer_context_t *ctx, uint8_t *buffer, ssize_t n)
{
  if (n < (ssize_t)sizeof(uint64_t)) {
    LOG_ERROR("Invalid packet size.");
    return;
  }

  uint64_t id = *(uint64_t*)buffer;
  
  while (send_pong(ctx, id))
    continue;

  ctx->flags |= 0x1; // My turn to send ping

  LOG_DEBUG("Ping received. ID: %llu", id);
}

void handle_pong(peer_context_t *ctx)
{
  ctx->flags &= 0xFE;

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  uint64_t ms = (uint64_t)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
  ctx->ping_delay |= (ms & 0xFFFFFFFF);
  
  LOG_DEBUG("Pong received. %llu ms.", (ctx->ping_delay & 0xFFFFFFFF) - ((ctx->ping_delay >> 32) & 0xFFFFFFFF));
}
