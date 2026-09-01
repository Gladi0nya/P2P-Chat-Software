#ifndef STATE_PEER_STATE_H
#define STATE_PEER_STATE_H

#include "stdint.h"

#include <netinet/in.h>

enum PeerState {
  PEER_ADDED,
  PEER_PUNCHING,
  PEER_ETABLISHED,
  PEER_DISCONNECTED
};

typedef enum PeerState peer_state_t;

struct PeerContext {
  int sock;
  uint16_t my_port;
  uint32_t my_pub_ip;  
  struct sockaddr_in peer_addr;
  peer_state_t state;
  uint64_t packet_id;
};

typedef struct PeerContext peer_context_t;

#endif
