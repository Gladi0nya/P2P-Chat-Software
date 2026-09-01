#include "hole_punch.h"

#include "logger/logger.h"

int hole_punch_send_one(peer_context_t *ctx, opcode_t opcode, uint64_t packet_id,
			const struct sockaddr *peer)
{

  uint8_t packet[sizeof(opcode_t) + sizeof(uint64_t)];

  *(opcode_t*)packet = opcode;
  *(uint64_t*)(packet + sizeof(opcode)) = packet_id;

  ssize_t sent = sendto(ctx->sock, packet, sizeof(packet), 0, peer, sizeof(*peer));

  if (sent < 0) {
    LOG_ERROR("sendto() failed.");
    return 1;
  }

  return 0;
}
