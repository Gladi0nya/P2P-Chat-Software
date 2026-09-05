#include "hole_punch.h"

#include "logger/logger.h"

#include <stdio.h>

struct __attribute__((packed)) PUNCH_PACKET {
  opcode_t op;
  uint64_t id;
};

typedef struct PUNCH_PACKET punch_packet_t;

int hole_punch_send_one(peer_context_t *ctx, opcode_t opcode)
{
  punch_packet_t packet = {
    .op = opcode,
    .id = ctx->packet_id++
  };

  LOG_DEBUG("Sending packet: op=0x%02X, id=%llu, size=%zu",
	    packet.op, (unsigned long long)packet.id, sizeof(packet));

  unsigned char *bytes = (unsigned char*)&packet;
  printf("Hex dump: ");
  for (size_t i = 0; i < sizeof(packet); i++) {
    printf("%02X ", bytes[i]);
  }
  printf("\n");
  fflush(stdout);
  
  ssize_t sent = sendto(ctx->sock, &packet, sizeof(packet), 0, (struct sockaddr*)&ctx->peer_addr, sizeof(ctx->peer_addr));

  if (sent < 0) {
    LOG_ERROR("sendto() failed.");
    return 1;
  }

  return 0;
}
