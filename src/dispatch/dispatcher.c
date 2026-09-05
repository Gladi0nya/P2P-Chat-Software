#include "dispatcher.h"

#include "handlers/handler_close.h"
#include "handlers/handler_punch.h"
#include "handlers/handler_ping.h"
#include "handlers/handler_data.h"

#include "logger/logger.h"

void dispatch_message(peer_context_t *ctx, opcode_t op, uint8_t *buffer, int n)
{
  switch (op) {
    case OP_PUNCH:     handle_punch(ctx, buffer, n); break;
    case OP_PUNCH_ACK: handle_punch_ack(ctx, buffer, n); break;
    case OP_PING:      handle_ping(ctx, buffer, n); break;
    case OP_PONG:      handle_pong(ctx); break;
    case OP_MSG:       handle_msg(ctx, buffer); break;
    case OP_CLOSE:     handle_close(ctx); break;
    default:
      LOG_WARNING("Unknown opcode: %d, from: %u.%u.%u.%u", op,
		   ctx->peer_addr.sin_addr.s_addr & 0xFF,
		  (ctx->peer_addr.sin_addr.s_addr >> 8) & 0xFF,
		  (ctx->peer_addr.sin_addr.s_addr >> 16) & 0xFF,
		  (ctx->peer_addr.sin_addr.s_addr >> 24) & 0xFF);
  }
}
