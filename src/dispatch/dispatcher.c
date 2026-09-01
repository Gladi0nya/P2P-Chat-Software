#include "dispatcher.h"

#include "handlers/handler_close.h"
#include "handlers/handler_punch.h"
#include "handlers/handler_data.h"

#include "logger/logger.h"

void dispatch_message(peer_context_t *ctx, opcode_t op, uint8_t *buffer, int n,
		      const struct sockaddr_in *from_addr, socklen_t addr_len)
{
  switch (op) {
    case OP_PUNCH: handle_punch(ctx, buffer, n, from_addr, addr_len); break;
    case OP_PUNCH_ACK: handle_punch_ack(ctx, buffer, n, from_addr, addr_len); break;
    case OP_MSG: handle_msg(ctx, buffer); break;
    case OP_CLOSE: handle_close(ctx); break;
    default:
      LOG_WARNING("Unknown opcode: %d.", op);
  }
}
