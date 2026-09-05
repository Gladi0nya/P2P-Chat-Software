#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "ping.h"

#include "protocol/message.h"

#include "state/peer_state.h"

#include "logger/logger.h"

#include <time.h>

struct PACKET_PING {
  opcode_t op;
  uint64_t id;
} __attribute__((packed));

typedef struct PACKET_PING packet_ping_t;

int send_ping(peer_context_t *const restrict ctx)
{
  packet_ping_t packet = {
    .op = OP_PING,
    .id = ctx->packet_id++
  };


  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  uint64_t ms = (uint64_t)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
  ctx->ping_delay = (ms & 0xFFFFFFFF) << 32; 
  
  ssize_t sent = sendto(ctx->sock, &packet, sizeof(packet), 0, (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr));

  if (sent < 0) {
    LOG_ERROR("sendto() failed.");
    return 1;
  }
  
  return 0;
}

int send_pong(peer_context_t *const restrict ctx, uint64_t id)
{ 
  packet_ping_t packet = {
    .op = OP_PONG,
    .id = id
  };

  ssize_t sent = sendto(ctx->sock, &packet, sizeof(packet), 0, (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr));
  
  if (sent < 0) {
    LOG_ERROR("sendto() failed.");
    return 1;
  }

  ctx->state |= 0x1; // My turn to send ping next time
  
  return 0;
}
