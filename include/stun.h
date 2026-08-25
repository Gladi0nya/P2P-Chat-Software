/**
 * @file stun.h
 * @brief STUN (Service Traversal Utilities for NAT) header
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#ifndef STUN_H
#define STUN_H

#define _POSIX_C_SOURCE 200809L 

#include "net_type.h"

#include <stdint.h>

int stun_check(uint32_t* pub_ip);
int stun_bind_sock(int* sock, uint16_t* pub_port);
#endif
