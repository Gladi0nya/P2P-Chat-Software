/**
 * @file  node.h
 * @brief Node header
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#ifndef NODE_H
#define NODE_H
#include <stdint.h>

#include "net_type.h"

uint8_t CreateChannelForPeer(int sock, addr_t peer_addr);

#endif
