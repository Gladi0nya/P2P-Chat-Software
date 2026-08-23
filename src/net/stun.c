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

#include "random.h"

#include "logger.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define STUN_MAGIC_COOKIE    0x2112A442
#define STUN_BINDING_REQUEST 0x0001

// Response Macros
#define STUN_BINDING_SUCCESS 0x0101
#define STUN_BINDING_ERROR   0x0110

struct STUN_HEADER {
  uint16_t msg_type;
  uint16_t msg_len;
  uint32_t magic_cookie;
  uint32_t transaction_id[3];
};

typedef struct STUN_HEADER stun_header_t;

struct STUN_HOST {
  const char* const restrict hostname;
  const uint16_t                 port;
};

typedef struct STUN_HOST stun_host_t;

#define STUN_SERVER_COUNT 2

static const stun_host_t STUN_SERVERS[] = {
  {"stun.l.google.com", 19302},
  {"stun.cloudflare.com", 3478}
};

void generate_transaction_id(uint32_t* const transaction_id)
{
  transaction_id[0] = rnd32();
  transaction_id[1] = rnd32();
  transaction_id[2] = rnd32();
}

int stun_request(void)
{
  uint8_t stun_server_index = 0;
  int sock;
  struct sockaddr_in server_addr, local_addr;
  stun_header_t request;
  uint8_t response[1024];
  socklen_t addr_len = sizeof(server_addr);
  ssize_t received;

  
  start:
  if (resolve_domain(STUN_SERVERS[stun_server_index].hostname, &server_addr)) {
    LOG_ERROR("resolve_domain() failed.");
    if (++stun_server_index == STUN_SERVER_COUNT) {
      LOG_ERROR("No stun host working. Aborting.");
      return 1;
    }
    
    goto start;
  }

  LOG_INFO("STUN server ip resolved.");

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    LOG_ERROR("Socket creation error.");
    return 1;
  }

  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;              
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(4444);           

  if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    LOG_ERROR("bind() failed.");

    close(sock);
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons(STUN_SERVERS[stun_server_index].port);

  request.msg_type     = htons(STUN_BINDING_REQUEST);
  request.msg_len      = 0;
  request.magic_cookie = htonl(STUN_MAGIC_COOKIE);

  generate_transaction_id(request.transaction_id);
  
  if (sendto(sock, &request, sizeof(request), 0,
	     (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    LOG_ERROR("sendto() failed.");

    close(sock);
    return 1;
  }

  LOG_INFO("BINDING request sent.");

  if ((received = recvfrom(sock, response, sizeof(response), 0,
			   (struct sockaddr*)&server_addr, &addr_len)) < 0) {
    LOG_ERROR("recvfrom() failed.");

    close(sock);
    return 1;
  }

  LOG_INFO("BINDING response received.");

  stun_header_t *resp_header = (stun_header_t*)response;
  uint16_t resp_type         = ntohs(resp_header->msg_type);
  uint32_t resp_cookie       = ntohl(resp_header->magic_cookie);
    
  if (resp_cookie != 0x2112A442) {
    LOG_ERROR("Incorrect magic cookie.");

    close(sock);
    return 1;
  }

  if (resp_type == STUN_BINDING_SUCCESS) {
    LOG_INFO("Binding success.");
  } else if ((resp_type & 0x0110) == STUN_BINDING_ERROR) {
    LOG_ERROR("Server error response.");
    return 1;
  } else {
    LOG_ERROR("Unknwon server msg_type.");
    return 1;
  }

  uint16_t msg_len = ntohs(*(uint16_t*)&response[2]);
  size_t offset = 20;
  int found_public_ip;

  while (offset < 20 + (size_t)msg_len && offset + 4 < (size_t)received) {
    uint16_t attr_type = ntohs(*(uint16_t*)&response[offset]);
    uint16_t attr_len = ntohs(*(uint16_t*)&response[offset + 2]);
    offset += 4;
        
    if (attr_type == 0x0020) {  // XOR-MAPPED-ADDRESS
      if (offset + 4 > (size_t)received) break;
            
      // Skip first byte (reserved) and the second (family)
      offset += 2;
            
      // Port (16 bits XOR with highest bits of the MAGIC_COOKIE)
      uint16_t pub_port = ntohs(*(uint16_t*)&response[offset]) ^ (STUN_MAGIC_COOKIE >> 16);
      offset += 2;
            
      // IP (XOR with the magic cookie)
      uint32_t pub_ip = *(uint32_t*)&response[offset] ^ htonl(STUN_MAGIC_COOKIE);
      offset += 4;
            
      struct in_addr addr;
      addr.s_addr = pub_ip;
      char pub_ip_str[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &addr, pub_ip_str, sizeof(pub_ip_str));
            
      printf("Public IP: %s\n", pub_ip_str);
      printf("Public Port: %u\n", pub_port);
      
      found_public_ip = 1;
      //break;
    } else {
      // Skip attribute
      offset += attr_len;
      // Aligning on 4 bytes
      offset += (4 - offset % 4) % 4;
    }
  }
  
  if (!found_public_ip) {
    LOG_WARNING("No XOR-MAPPED-ADDRESS attribute found in response");
    return 2;
  }
  
  close(sock);
  return 0;
}
