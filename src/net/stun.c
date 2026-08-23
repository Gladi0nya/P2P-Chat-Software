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

#include <errno.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define DEFAULT_PORT             12345
#define TIMEOUT_SEC              3

#define STUN_MAGIC_COOKIE        0x2112A442
#define STUN_BINDING_REQUEST     0x0001

#define STUN_BINDING_SUCCESS     0x0101
#define STUN_BINDING_ERROR       0x0110

struct STUN_PACKET {
  uint16_t msg_type;
  uint16_t msg_len;
  uint32_t magic_cookie;
  uint32_t transaction_id[3];
};

typedef struct STUN_PACKET stun_packet_t;

#define STUN_SERVER_COUNT 2

struct STUN_HOST {
  char*    hostname;
  uint16_t     port;
};

typedef struct STUN_HOST stun_host_t;

static const stun_host_t STUN_SERVERS[] = {
  {"stun.l.google.com", 19302},
  {"stun.cloudflare.com", 3478},
  {"stun1.l.google.com", 3478},
  {"stun2.l.google.com", 19302},
  {"stun3.l.google.com", 3478},
  {"stun4.l.google.com", 19302},
};

static inline void generate_transaction_id(uint32_t* const restrict transaction_id)
{
  transaction_id[0] = rnd32();
  transaction_id[1] = rnd32();
  transaction_id[2] = rnd32();
}


static int extract_xor_mapped_address(const uint8_t* response, size_t recv_len,
                                      uint32_t* ip, uint16_t* port) {
    uint16_t msg_len = ntohs(*(uint16_t*)&response[2]);
    size_t offset    = 20;
    int found        = 0;

    while (offset < (size_t)20 + msg_len && offset + (size_t)4 <= recv_len) {
        uint16_t attr_type = ntohs(*(uint16_t*)&response[offset]);
        uint16_t attr_len  = ntohs(*(uint16_t*)&response[offset + 2]);
        offset += 4;

        if (attr_type == 0x0020) {  // XOR-MAPPED-ADDRESS                                                                                                                                           
            if (offset + 4 > recv_len) break;

            // Skip first byte (reserved) and the second (family)                                                                                                                                   
            offset += 2;

            // Port (16 bits XOR with highest bits of Magic Cookie)                                                                                                                                 
            *port = ntohs(*(uint16_t*)&response[offset]) ^ (STUN_MAGIC_COOKIE >> 16);
            offset += 2;

            // IP (XOR with full Magic Cookie)                                                                                                                                                      
            *ip = *(uint32_t*)&response[offset] ^ htonl(STUN_MAGIC_COOKIE);
            offset += 4;

            found = 1;
            break;
        } else {
            offset += attr_len;
            offset += (4 - offset % 4) % 4;
        }
    }
  
    return found ? 0 : 1;
}

static int stun_request(int sock, const struct sockaddr_in* const restrict dest) {
  stun_packet_t    request     = { 0 };
  size_t           request_len = 20; // header only

  request.msg_type     = htons(STUN_BINDING_REQUEST);
  request.magic_cookie = htonl(STUN_MAGIC_COOKIE);

  generate_transaction_id(request.transaction_id);

  if (sendto(sock, &request, request_len, 0,
	     (const struct sockaddr*)dest, sizeof(struct sockaddr_in)) < 0) {
    LOG_ERROR("sendto() failed.");

    return 1;
  }

  return 0;
}

static int stun_receive(int sock, uint8_t* response, size_t* recv_len,
			uint16_t expected_type) {
  struct sockaddr_in from_addr;
  uint32_t cookie;
  uint16_t resp_type;
  ssize_t sent;
  socklen_t from_len = sizeof(from_addr);
  struct timeval tv  = {TIMEOUT_SEC, 0};

  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  if ((sent = recvfrom(sock, response, 1024, 0,
		      (struct sockaddr*)&from_addr, &from_len)) < 0) {

    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      LOG_ERROR("Timeout waiting for STUN response");
    } else {
      LOG_ERROR("recvfrom() failed.");
    }
    
    return 1;
  }

  *recv_len = (size_t)sent;
  
  if (*recv_len < 20) {
    LOG_ERROR("Response too short.");

    return 1;
  }

  cookie = ntohl(*(uint32_t*)&response[4]);
  if (cookie != STUN_MAGIC_COOKIE) {
    LOG_ERROR("Invalid magic cookie.");

    return 1;
  }
    
  resp_type = ntohs(*(uint16_t*)&response[0]);
  if (resp_type != expected_type) {
    LOG_ERROR("Unexpected response type.");
    return 1;
  }
    
  return 0;
}

int stun_bind_sock(int* sock, addr_t* pub_addr)
{
  uint8_t pass = 0;
  struct sockaddr_in server_addr, local_addr;
  
  uint8_t response[1024];
  size_t recv_len;
  addr_t pub[2];

  if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    LOG_ERROR("Socket creation error.");

    return 1;
  }           

  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;              
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (pub_addr->port < 1001)
    local_addr.sin_port = htons(DEFAULT_PORT);
  else
    local_addr.sin_port = htons(pub_addr->port);
  
  if (bind(*sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    LOG_ERROR("bind() failed.");

    close(*sock);
    return 1;
  }

  LOG_INFO("ip, port bound to socket.");
  
  next_pass:
  if (resolve_domain(STUN_SERVERS[pass].hostname, &server_addr)) {
    LOG_ERROR("resolve_domain() failed.");
    
    if (++pass == STUN_SERVER_COUNT) {
      LOG_ERROR("No stun host working. Aborting.");
      return 1;
    }
    
    goto next_pass;
  }

  LOG_INFO("STUN server ip resolved.");

  //printf("IP: %s\n", inet_ntoa(server_addr.sin_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons(STUN_SERVERS[pass].port);
  
  if (stun_request(*sock, &server_addr)) {
    LOG_ERROR("stun_request() failed.");

    close(*sock);
    return 1;
  }

  LOG_INFO("BINDING request sent.");
  
  if (stun_receive(*sock, response, &recv_len, STUN_BINDING_SUCCESS)) {
    LOG_ERROR("stun_receive() failed.");

    close(*sock);
    return 1;
  }

  LOG_INFO("BINDING response received.");

  if (extract_xor_mapped_address(response, recv_len, &pub[pass].ip, &pub[pass].port)) {
    LOG_ERROR("Failed to extract XOR-MAPPED-ADDRESS from Test 1");

    close(*sock);
    return 1;
  }

  if (++pass < STUN_SERVER_COUNT) goto next_pass;

  if (memcmp(&pub[0], &pub[1], sizeof(addr_t))) {
    LOG_WARNING("Detected public IP change. Aborting.\n");

    return 1;
  }

  pub_addr->ip   = pub[0].ip;
  pub_addr->port = pub[0].port;

  LOG_DEBUG("Friendly P2P NAT detected.");
  
  return 0;
}
