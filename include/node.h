#ifndef NODE_H
#define NODE_H
#include <stdint.h>

#include "stun.h"

uint8_t CreateChannelForPeer(int sock, addr_t peer_addr);

#endif
