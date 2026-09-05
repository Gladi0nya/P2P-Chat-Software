#include "handler_punch.h"

#include "net/hole_punch.h"
#include "protocol/message.h"
#include "logger/logger.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <sys/socket.h>
#endif

void handle_punch(peer_context_t *ctx, const uint8_t *buffer, int n)
{
  if (ctx->state == PEER_ETABLISHED) {
    LOG_DEBUG("Received punch, but peer connection already etablished");
    return;
  }

  if ((long unsigned int)n < sizeof(uint64_t)) {
    LOG_DEBUG("Received invalid punch request.");
    return;
  }

  ctx->packet_id = *(uint64_t*)buffer;
  
  LOG_DEBUG("punch ID: %llu", ctx->packet_id);
  
  hole_punch_send_one(ctx, OP_PUNCH_ACK);
}

void handle_punch_ack(peer_context_t *ctx, const uint8_t *buffer, int n)
{
  if (ctx->state == PEER_ETABLISHED) {
    LOG_DEBUG("Received punch ack, but peer connection already etablished");
    return;
  }
  
  if ((long unsigned int)n < sizeof(uint64_t)) {
    LOG_DEBUG("Received invalid punch request.");
    return;
  }

  ctx->state = PEER_ETABLISHED;
  
  LOG_INFO("Connected to %u.%u.%u.%u",
	   ((ctx->peer_addr.sin_addr.s_addr      ) & 0xFF),
	   ((ctx->peer_addr.sin_addr.s_addr >>  8) & 0xFF),
	   ((ctx->peer_addr.sin_addr.s_addr >> 16) & 0xFF),
	   ((ctx->peer_addr.sin_addr.s_addr >> 24) & 0xFF));

  ctx->packet_id = *(uint64_t*)buffer;
  
  LOG_DEBUG("punch ack ID: %llu", ctx->packet_id);
  
  hole_punch_send_one(ctx, OP_PUNCH_ACK);
}
