/**
 * @file stun.c
 * @brief STUN (Session Traversal Utilities for NAT) module
 *
 * @author Tom Schmitt
 * @author Augustin Barniet
 *
 * Copyright (c) 2026 Tom Schmitt, Augustin Barniet
 * All rights reserved.
 *
 */

#include "stun.h"

struct STUN_HOST {
  const char* const restrict hostname;
  const int                  port;
};

typedef struct STUN_HOST STUN_HOST_T;

static const STUN_HOST STUN_SERVERS[] = {
  {"stun.l.google.com", 19302},
  {"stun.cloudflare.com", 3478},
  {"stun.stunprotocol.org", 3478}
};

