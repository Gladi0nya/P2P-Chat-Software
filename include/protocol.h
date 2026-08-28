#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "generic_type.h"

enum OPCODE {
  OP_PUNCH,
  OP_PUNCH_ACK,
  OP_MSG,
  OP_CLOSE,
  //OP_CRYPTSHAKE,
  //OP_PEER_RENDEZVOUS,  // Open new connection for current peer
  //OP_RELAY_RENDEZVOUS, // Ask to be contacted by other peer
  //OP_NODE_DISCOVERY,   // Initially used + gossip each delta t
  //OP_PING,
  //OP_PONG,
  //OP_DATA,
  //OP_DISCONNECT,
  //OP_VERSION,
  //OP_ASK_UPDATE,
  //OP_SEND_UPDATE,
  //OP_PARANOIA
};

typedef enum OPCODE opcode_t;

struct PACKET {
  opcode_t op;
  byte data[];
};

typedef struct PACKET packet_t;
#endif
