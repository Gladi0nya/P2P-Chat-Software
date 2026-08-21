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

#include "host.h"
#include "stun.h"

#include "logger.h"

#include <stdio.h>
#include <string.h>

#include <sys/socket.h>

#define STUN_MAGIC_COOKIE 0x2112A442

// According to https://www.rfc-editor.org/rfc/rfc8489.html, Appendix A

// Given a 16-bit STUN message type value in host byte order in msg_type
// parameter, below are C macros to determine the STUN message types.

#define IS_REQUEST(msg_type)       (((msg_type) & 0x0110) == 0x0000)
#define IS_INDICATION(msg_type)    (((msg_type) & 0x0110) == 0x0010)
#define IS_SUCCESS_RESP(msg_type)  (((msg_type) & 0x0110) == 0x0100)
#define IS_ERR_RESP(msg_type)      (((msg_type) & 0x0110) == 0x0110)

// A function to convert method and class into a message type.

static inline int type(int method, int cls) {
  return (method & 0x1F80) << 2 | (method & 0x0070) << 1
       | (method & 0x000F) | (cls & 0x0002) << 7
       | (cls & 0x0001) << 4;
}

// A function to extract the method from the message type.

static inline int method(int type) {
     return (type & 0x3E00) >> 2 | (type & 0x00E0) >> 1
          | (type & 0x000F);
}

// A function to extract the class from the message type.

static inline int cls(int type) {
  return (type & 0x0100) >> 7 | (type & 0x0010) >> 4;
}

struct STUN_HOST {
  const char* const restrict hostname;
  const uint16_t                 port;
};

typedef struct STUN_HOST STUN_HOST_T;

#define STUN_SERVER_COUNT 2

static const STUN_HOST_T STUN_SERVERS[] = {
  {"stun.l.google.com", 19302},
  {"stun.cloudflare.com", 3478}
};

int stun_request(void)
{
  int sock;
  uint8_t stun_server_index = 0;
  ipv4_t server_ip;
  //struct sockaddr_in server_addr;

  start:
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    LOG_ERROR("Failed to create socket.");

    return 1;
  }

  LOG_DEBUG("Socket created.");

  //memset(&server_addr, 0, sizeof(server_addr));

  //server_addr.sin_family = AF_INET;
  //server_addr.sin_port   = htons(STUN_SERVERS[stun_server_index].port);

  if (resolve_domain(STUN_SERVERS[stun_server_index].hostname, &server_ip)) {
    LOG_ERROR("resolve_domain() failed.");
    if (++stun_server_index == STUN_SERVER_COUNT) {
      LOG_ERROR("No stun host working. Aborting.");
      return 1;
    }
    
    goto start;
  }

  LOG_INFO("STUN server ip resolved.");
  

  return 0;
}
