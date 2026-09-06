#include "hole_punch.h"

#include "state/peer_state.h"

#include "logger/logger.h"

struct PUNCH_PACKET {
  opcode_t op;
  uint64_t id;
} __attribute__((packed));

typedef struct PUNCH_PACKET punch_packet_t;

struct PUNCH_PACKET_ACK {
  opcode_t op;
  uint64_t id;
  uint64_t random;
} __attribute__((packed));

typedef struct PUNCH_PACKET_ACK punch_packet_ack_t;

int hole_punch_send_one(peer_context_t *ctx)
{
  punch_packet_t packet = {
    .op = OP_PUNCH,
    .id = ctx->packet_id++
  };
  
  ssize_t sent = sendto(ctx->sock, &packet, sizeof(packet), 0, (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr));

  if (sent < 0) {
    LOG_ERROR("sendto() failed.");
    return 1;
  }

  return 0;
}

int hole_punch_send_ack(peer_context_t *ctx, uint64_t id)
{
  punch_packet_ack_t packet = {
    .op = OP_PUNCH_ACK,
    .id = id,
    .random = ctx->random
  };

  ssize_t sent = sendto(ctx->sock, &packet, sizeof(packet), 0, (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr));

  if (sent < 0) {
    LOG_ERROR("sendto() failed.");
    return 1;
  }

  return 0;
}
