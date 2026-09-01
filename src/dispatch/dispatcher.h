#ifndef DISPATCH_DISPATCHER_H
#define DISPATCH_DISPATCHER_H

#include "state/peer_state.h"
#include "protocol/message.h"

#include <netinet/in.h>

void dispatch_message(peer_context_t *ctx, opcode_t op, uint8_t *buffer, int n,
		      const struct sockaddr_in *from_addr, socklen_t addr_len);
#endif
