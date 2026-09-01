#include "handler_punch.h"
#include "protocol/message.h"
#include "logger/logger.h"

#include <sys/socket.h>

void handle_punch(peer_context_t *ctx, const uint8_t *buffer, int n,
		  const struct sockaddr_in *from_addr, socklen_t addr_len)
{
  if (ctx->state == PEER_ETABLISHED) {
    LOG_DEBUG("Received punch, but peer connection already etablished");
    return;
  }

  if ((long unsigned int)n < sizeof(opcode_t) + sizeof(uint64_t)) {
    LOG_DEBUG("Received invalid punch request.");
    return;
  }
  
  uint64_t id = *(const uint64_t*)(buffer + sizeof(opcode_t));
  LOG_DEBUG("punch ID: %llu", (unsigned long long)id);

  uint8_t packet[sizeof(opcode_t) + sizeof(uint64_t)];
  *(opcode_t*)packet = OP_PUNCH_ACK;
  *(uint64_t*)(packet + sizeof(opcode_t)) = id;

  if (!sendto(ctx->sock, packet, sizeof(packet), 0,
	      (const struct sockaddr*)from_addr, addr_len))
    LOG_DEBUG("Failed to send punch request to %u.%u.%u.%u:%u.",
	      (ctx->peer_addr.sin_addr.s_addr      ) & 0xFF,
	      (ctx->peer_addr.sin_addr.s_addr >>  8) & 0xFF,
	      (ctx->peer_addr.sin_addr.s_addr >> 16) & 0xFF,
	      (ctx->peer_addr.sin_addr.s_addr >> 24) & 0xFF,	      
	      ctx->peer_addr.sin_port);
  
}

void handle_punch_ack(peer_context_t *ctx, const uint8_t *buffer, int n,
		      const struct sockaddr_in *from_addr, socklen_t addr_len)
{ 
  if (ctx->state == PEER_ETABLISHED) {
    LOG_DEBUG("Received punch ack, but peer connection already etablished");
    return;
  }
  
  if ((long unsigned int)n < sizeof(opcode_t) + sizeof(uint64_t)) {
    LOG_DEBUG("Received invalid punch request.");
    return;
  }

  ctx->state = ETABLISHED;
  
  uint64_t id = *(const uint64_t*)(buffer + sizeof(opcode_t));
  LOG_DEBUG("punch ack ID: %llu", (unsigned long long)id);

  uint8_t packet[sizeof(opcode_t) + sizeof(uint64_t)];
  *(opcode_t*)packet = OP_PUNCH_ACK;
  *(uint64_t*)(packet + sizeof(opcode_t)) = id;

  if (!sendto(ctx->sock, packet, sizeof(packet), 0,
	      (const struct sockaddr*)from_addr, addr_len))
    LOG_DEBUG("Failed to send punch request to %u.%u.%u.%u:%u.",
	      (ctx->peer_addr.sin_addr.s_addr >> 24) & 0xFF,
	      (ctx->peer_addr.sin_addr.s_addr >> 16) & 0xFF,
	      (ctx->peer_addr.sin_addr.s_addr >>  8) & 0xFF,
	      (ctx->peer_addr.sin_addr.s_addr      ) & 0xFF,
	      ctx->peer_addr.sin_port);
  
}
