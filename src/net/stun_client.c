/**
 * @file stun_client.c
 * @brief STUN (Session Traversal Utilities for NAT) module
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#include "stun_client.h"
#include "addr.h"
#include "udp_socket.h"

#include "crypto/random.h"

#include "logger/logger.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <arpa/inet.h>
#include <sys/socket.h>

#include <unistd.h>
#endif
#include <sys/time.h>

#define DEFAULT_PORT  12345
#define TIMEOUT_SEC       2
#define PASS_COUNT        2
#define MAX_RETRY_CHECK   3

#define STUN_MAGIC_COOKIE       0x2112A442

#define STUN_BINDING_REQUEST    0x0001
#define STUN_BINDING_SUCCESS    0x0101
#define STUN_BINDING_ERROR      0x0110

#define STUN_XOR_MAPPED_ADDRESS 0x0020

// Structure for IPv4 address
struct ADDR {
  uint16_t port;
  uint32_t ip;
};

// IPv4 address type
typedef struct ADDR addr_t;

// Structure for STUN packets
struct STUN_PACKET {
  uint16_t msg_type;
  uint16_t msg_len;
  uint32_t magic_cookie;
  uint32_t transaction_id[3];
};

// STUN packet type
typedef struct STUN_PACKET stun_packet_t;

// Structure for STUN hosts
struct STUN_HOST {
  char*    hostname;
  uint16_t     port;
};

// STUN host type
typedef struct STUN_HOST stun_host_t;

// Array of STUN servers
static const stun_host_t STUN_SERVERS[] = {
//  {"stun.miwifi.com", 3478},
//  {"stun.sipthor.net", 3478},
//  {"stun.freeswitch.org", 3478},
//  {"stun.chat.bilibili.com", 3478},
  {"stun.l.google.com", 19302},
  {"stun.cloudflare.com", 3478},
  {"stun1.l.google.com", 3478},
  {"stun2.l.google.com", 19302},
  {"stun3.l.google.com", 3478},
  {"stun4.l.google.com", 19302},
};

#define STUN_SERVER_COUNT (sizeof(STUN_SERVERS) / sizeof(STUN_SERVERS[0]))

/** ----------------------------------------------------------- *
  *  generate_transaction_id                                    *
  *                                                             *
  *  Built 96 bytes of random data for STUN transaction id.     *
  *                                                             *
  *  @param transaction_id [out] Byte to write random data.     *
  *                                                             *
  *  @retval 0 Succesfully generated.                           *
  *  @retval 1 RNG failed.                                      * 
  * ----------------------------------------------------------- **/

static inline int generate_transaction_id(uint32_t* const restrict transaction_id)
{
  return rnds32(&transaction_id[0]) & rnds32(&transaction_id[1]) & rnds32(&transaction_id[2]);
}

/** ----------------------------------------------------------- *
  *  extract_xor_mapped_address                                 *
  *                                                             *
  *  Extract client address from STUN response.                 *
  *                                                             *
  *  @param response [in] STUN response data.                   *
  *  @param recv_len [in] STUN response length.                 *
  *  @param ip      [out] Client Public IP found.               *
  *  @param port    [out] Client Public PORT found.             *
  *                                                             *
  *  @retval 0 Public address found in the response.            *
  *  @retval 1 No address found in the response.                *
  * ----------------------------------------------------------- **/

static int extract_xor_mapped_address(const uint8_t* const restrict response, const size_t recv_len,
                                      uint32_t* const restrict ip, uint16_t* const restrict port) {
    uint16_t msg_len = ntohs(*(uint16_t*)&response[2]);
    size_t offset    = 20;
    int found        = 0;

    while (offset < (size_t)20 + msg_len && offset + (size_t)4 <= recv_len) {
        uint16_t attr_type = ntohs(*(uint16_t*)&response[offset]);
        uint16_t attr_len  = ntohs(*(uint16_t*)&response[offset + 2]);
        offset += 4;

        if (attr_type == STUN_XOR_MAPPED_ADDRESS) {  // XOR-MAPPED-ADDRESS                                                                                                                                           
            if (offset + 4 > recv_len) break;

            // Skip first byte (reserved) and the second (family)                                                                                                                                   
            offset += 2;

            // Port (16 bits XOR with highest bits of Magic Cookie)

	    if (port)
	      *port = ntohs(*(uint16_t*)&response[offset]) ^ (STUN_MAGIC_COOKIE >> 16);

	    offset += 2;

            // IP (XOR with full Magic Cookie)                                                                                                                                                      
            if (ip)
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

/** ----------------------------------------------------------- *
  *  stun_client_request                                        *
  *                                                             *
  *  Send a request to a STUN server.                           *
  *                                                             *
  *  @param sock        [in] network socket.                    *
  *  @param sockaddr_in [in] destination address.               *
  *                                                             *
  *  @retval 0 Request sent.                                    *
  *  @retval 1 Request not sent due to error.                   *
  * ----------------------------------------------------------- **/

static int stun_client_request(int sock, const struct sockaddr_in* const restrict dest) {
  stun_packet_t    request     = { 0 };

  request.msg_type     = htons(STUN_BINDING_REQUEST);
  request.magic_cookie = htonl(STUN_MAGIC_COOKIE);

  if (generate_transaction_id(request.transaction_id)) {
      LOG_DEBUG("generate_transaction_id failed().");
      return 1;
  }

  if (sendto(sock, &request, sizeof(request), 0,
	     (const struct sockaddr*)dest, sizeof(struct sockaddr_in)) < 0) {
    LOG_DEBUG("sendto() failed.");

    return 1;
  }

  return 0;
}

/** ----------------------------------------------------------- *
  *  stun_client_receive                                        *
  *                                                             *
  *  Receive STUN response from socket.                         *
  *                                                             *
  *  @param sock          [in] network socket.                  *
  *  @param response     [out] buffer to store response.        *
  *  @param recv_len     [out] Length of the response.          *
  *                                                             *
  *  @retval 0 Response received.                               *
  *  @retval 1 Failed to retrieve the response.                 *
  * ----------------------------------------------------------- **/

static int stun_client_receive(const int sock, uint8_t* const restrict response, size_t* const restrict recv_len) {
  struct sockaddr_in from_addr;
  struct timeval tv  = {TIMEOUT_SEC, 0};

  uint32_t cookie;
  uint16_t resp_type;
  ssize_t sent;
  socklen_t from_len = sizeof(from_addr);

  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  if ((sent = recvfrom(sock, response, 1024, 0,
		      (struct sockaddr*)&from_addr, &from_len)) < 0) {

    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      LOG_WARNING("Timeout waiting for STUN response");
    } else {
      LOG_DEBUG("recvfrom() failed.");
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
  if (resp_type != STUN_BINDING_SUCCESS) {
    LOG_ERROR("Unexpected response type.");
    return 1;
  }
    
  return 0;
}

static int stun_query(int sock, uint8_t *used_idx, uint8_t *response, size_t *recv_len)
{
  const uint8_t start_idx = *used_idx; 
  
  for (uint8_t i = start_idx; i < STUN_SERVER_COUNT; i++) {
    struct sockaddr_in server_addr;

    if (net_addr_resolve(STUN_SERVERS[i].hostname, &server_addr)) {
      LOG_DEBUG("net_addr_resolve() failed for %s.", STUN_SERVERS[i].hostname);
      continue;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(STUN_SERVERS[i].port);

    if (stun_client_request(sock, &server_addr)) {
      LOG_DEBUG("stun_request() failed.");
      continue;
    }

    if (stun_client_receive(sock, response, recv_len) == 0) {
      *used_idx = i;
      return 0;
    } else {
      LOG_DEBUG("stun_client_receive() failed for %s.", STUN_SERVERS[i].hostname);
    }
  }

  LOG_ERROR("No STUN host responded from index %u", *used_idx);
  return 1;
}

int stun_client_check(uint32_t* const restrict pub_ip)
{
  int sock = -1;
  uint8_t response[1024];
  size_t recv_len;
  addr_t pub[PASS_COUNT] = {0};
  uint8_t idx = 0;

  struct sockaddr_in local_addr = {0};

  local_addr.sin_family      = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port        = htons(0);
  
  if (udp_socket_create(&sock)) {
    LOG_DEBUG("udp_socket_create() failed.");
    return 1;
  }
  
  if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    LOG_DEBUG("bind() failed.");
    udp_socket_close(&sock);
    return 1;
  }

  for (uint8_t retry = 0; retry < MAX_RETRY_CHECK; retry++) {
    for (uint8_t pass = 0; pass < PASS_COUNT; pass++) {

      if (stun_query(sock, &idx, response, &recv_len)) {
	udp_socket_close(&sock);
	return 1;
      }
      
      if (extract_xor_mapped_address(response, recv_len, &pub[pass].ip, &pub[pass].port)) {
	LOG_DEBUG("Failed to parse XOR-MAPPED-ADDRESS.");
	udp_socket_close(&sock);
	return 1;
      }
      
      idx++;
    }
    
    if (memcmp(&pub[0].ip, &pub[1].ip, 4) || memcmp(&pub[0].port, &pub[1].port, 2)) {
      LOG_WARNING("Detected public IP change.");
      LOG_DEBUG("%u.%u.%u.%u:%u | %u.%u.%u.%u:%u",
		 pub[0].ip       & 0xFF,
		(pub[0].ip >> 8) & 0xFF,
		(pub[0].ip >> 16) & 0xFF,
		(pub[0].ip >> 24) & 0xFF,
		 pub[0].port,
		 pub[1].ip       & 0xFF,
		(pub[1].ip >> 8) & 0xFF,
		(pub[1].ip >> 16) & 0xFF,
		(pub[1].ip >> 24) & 0xFF,
		pub[1].port);
    } else break;
  }

  if (memcmp(&pub[0].ip, &pub[1].ip, 4) || memcmp(&pub[0].port, &pub[1].port, 2)) {
    udp_socket_close(&sock);
    LOG_DEBUG("%u.%u.%u.%u:%u | %u.%u.%u.%u:%u",
		 pub[0].ip       & 0xFF,
		(pub[0].ip >> 8) & 0xFF,
		(pub[0].ip >> 16) & 0xFF,
		(pub[0].ip >> 24) & 0xFF,
		 pub[0].port,
		 pub[1].ip       & 0xFF,
		(pub[1].ip >> 8) & 0xFF,
		(pub[1].ip >> 16) & 0xFF,
		(pub[1].ip >> 24) & 0xFF,
		pub[1].port);
    return 1;
  }
  
  *pub_ip = pub[0].ip;
  
  LOG_INFO("Friendly P2P NAT detected.");
  udp_socket_close(&sock);
  return 0;
}

/** ----------------------------------------------------------- *
  *  stun_client_bind_sock                                      *
  *                                                             *
  *  Bind a socket and retrieve public port.                    *
  *                                                             *
  *  @param socket   [out] socket to be created.                *
  *  @param pub_port [out] Public port.                         *
  *                                                             *
  *  @retval 0 Succesfull binding.                              *
  *  @retval 1 Error.                                           *
  * ----------------------------------------------------------- **/

int stun_client_bind_sock(int* const restrict sock, uint16_t* const restrict pub_port)
{
  uint8_t idx = 0;
  uint8_t response[1024];
  size_t recv_len;

  struct sockaddr_in local_addr = {0};
  
  local_addr.sin_family      = AF_INET;              
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port        = *pub_port < 1001 ? htons(DEFAULT_PORT)
                                                : htons(*pub_port);
  
  if (bind(*sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    LOG_DEBUG("bind() failed.");
    return 1;
  }
  
  LOG_INFO("Localhost socket-bound.");

  if (stun_query(*sock, &idx, response, &recv_len))
    return 1;

  if (extract_xor_mapped_address(response, recv_len, NULL, pub_port)) {
    LOG_ERROR("extract_xor_mapped_address() failed.");
    return 1;
  }

  return 0;
}
