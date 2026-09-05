#include "handler_punch.h"

#include "net/hole_punch.h"
#include "net/ping.h"

#include "protocol/message.h"
#include "logger/logger.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <sys/socket.h>
#endif

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#include <endian.h>
#endif

void handle_punch(peer_context_t *ctx, const uint8_t *buffer, int n)
{
  uint64_t peer_packet_id;
  
  if (ctx->state == PEER_ETABLISHED) {
    LOG_DEBUG("Received punch, but peer connection already etablished");
    return;
  }

  if (n < (ssize_t)sizeof(uint64_t)) {
    LOG_DEBUG("Received invalid punch request.");
    return;
  }

  #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
  peer_packet_id = ntohll(*(uint64_t*)buffer);
  #else
  peer_packet_id = *(uint64_t*)buffer;
  #endif
  
  LOG_DEBUG("punch ID: %llu", peer_packet_id);
  
  hole_punch_send_ack(ctx, peer_packet_id);
}

void handle_punch_ack(peer_context_t *ctx, const uint8_t *buffer, int n)
{
  uint64_t peer_packet_id, peer_random;
  
  if (ctx->state != PEER_ETABLISHED) {
    LOG_DEBUG("Received punch ack, but peer connection already etablished");
    return;
  }
  
  if (n < (ssize_t)sizeof(uint64_t) * 2) {
    LOG_DEBUG("Received invalid punch request.");
    return;
  }

  #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
  peer_packet_id = ntohll(*(uint64_t*)buffer);
  peer_random    = ntohll(*(uint64_t*)(buffer + sizeof(uint64_t)));
  #else
  peer_packet_id = *(uint64_t*)buffer;
  peer_random    = *(uint64_t*)(buffer + sizeof(uint64_t));
  #endif
  
  LOG_DEBUG("Received ACK ID: %llu", peer_packet_id);
  LOG_DEBUG("Received PEER RANDOM: %llu", peer_random);
  LOG_DEBUG("My RANDOM: %llu", ctx->random);

  if (ctx->random == peer_random) {
    peer_context_reset_random(ctx);
    hole_punch_send_ack(ctx, peer_packet_id);
    return;
  }

  hole_punch_send_ack(ctx, peer_packet_id);
  
  if (ctx->random > peer_random) // My turn to send ping
    ctx->flags |= 0x1;
  
  ctx->state = PEER_ETABLISHED;

  LOG_INFO("Connected to %u.%u.%u.%u",
	   ((ctx->peer_addr.sin_addr.s_addr      ) & 0xFF),
	   ((ctx->peer_addr.sin_addr.s_addr >>  8) & 0xFF),
	   ((ctx->peer_addr.sin_addr.s_addr >> 16) & 0xFF),
	   ((ctx->peer_addr.sin_addr.s_addr >> 24) & 0xFF));
}
