#ifndef HANDLER_PING_H
#define HANDLER_PING_H

#include "state/peer_state.h"
#include <stdint.h>

void handle_ping(peer_context_t *ctx, uint8_t *buffer, ssize_t n);
void handle_pong(peer_context_t *ctx);
#endif
