#ifndef DISPATCH_HANDLER_PUNCH
#define DISPATCH_HANDLER_PUNCH

#include "state/peer_state.h"

#include <netinet/in.h>

void handle_punch(peer_context_t *ctx, const uint8_t *buffer, int n,
		  const struct sockaddr_in *from_addr, socklen_t addr_len);

void handle_punch_ack(peer_context_t *ctx, const uint8_t *buffer, int n,
		      const struct sockaddr_in *from_addr, socklen_t addr_len);


#endif
