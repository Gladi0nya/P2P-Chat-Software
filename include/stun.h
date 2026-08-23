/**
 * @file stun.h
 * @brief STUN (Service Traversal Utilities for NAT) header
 *
 * @author Tom Schmitt
 * @author Augustin Barniet
 *
 * Copyright (c) 2026 Tom Schmitt, Augustin Barniet
 * All rights reserved.
 *
 */

#ifndef STUN_H
#define STUN_H

#define _POSIX_C_SOURCE 200809L 

struct ADDR {
  uint32_t ip;
  uint16_t port;
};

typedef struct ADDR addr_t;

int stun_bind_sock(int* sock, addr_t* pub_addr);
#endif
