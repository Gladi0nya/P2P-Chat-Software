#ifndef PING_H
#define PING_J

#include <sys/socket.h>
#include <stdint.h>

#include "state/peer_state.h"

int send_ping(peer_context_t *const restrict ctx);
int send_pong(peer_context_t *const restrict ctx, uint64_t id);

#endif
