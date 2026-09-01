#ifndef NET_HOLE_PUNCH_H
#define NET_HOLE_PUNCH_H

#include "protocol/message.h"
#include "state/peer_state.h"

#include <stdint.h>
#include <sys/socket.h>

int hole_punch_send_one(peer_context_t *ctx, opcode_t opcode, uint64_t packet_id,
			const struct sockaddr *peer);
#endif
