#ifndef STATE_PEER_STATE_H
#define STATE_PEER_STATE_H

#include "stdint.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>

#else
#include <netinet/in.h>

#endif

enum PeerState {
  PEER_UNDEFINED,
  PEER_ADDED,
  PEER_PUNCHING,
  PEER_ETABLISHED,
  PEER_DISCONNECTED
};

typedef enum PeerState peer_state_t;

struct PeerContext {
  int sock;
  uint8_t flags; // lowest bit: my turn to send ping 
  uint16_t my_port;
  uint32_t my_pub_ip;  
  struct sockaddr_in peer_addr;
  peer_state_t state;
  uint64_t packet_id;
  uint64_t random;
};

typedef struct PeerContext peer_context_t;

int peer_context_create(peer_context_t *const restrict ctx);
int peer_context_reset_random(peer_context_t *const restrict ctx);
int peer_context_set_port(peer_context_t *const restrict ctx, char* const restrict lport);
int peer_context_set_remote(peer_context_t *const restrict ctx, const char* const restrict rip, const char* const restrict rport);
int peer_context_close(peer_context_t *const restrict ctx);

#endif
