#ifndef NET_HOLE_PUNCH_H
#define NET_HOLE_PUNCH_H

#include "protocol/message.h"
#include "state/peer_state.h"

#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <sys/socket.h>
#endif

int hole_punch_send_one(peer_context_t *ctx);
int hole_punch_send_ack(peer_context_t *ctx, uint64_t id);
#endif
