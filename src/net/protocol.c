#include "protocol.h"

enum OPCODE {
  OP_HANDSHAKE,
  OP_PEER_RENDEZVOUS,    // Open new connection for current peer
  OP_FOREIGN_RENDEZVOUS, // Ask to be contacted by other peer
  OP_KEEPALIVE,
  OP_NODE_DISCOVERY, // Initially used + gossip each delta t
  OP_PING,
  OP_PONG,
  OP_DATA,
  OP_DISCONNECT
};

typedef enum opcode_t;

enum MSG_TYPE {
  PARANOIA_ANTIHACK,
  MSG_DATA,
  MUSIC_DATA,
  AUDIO_DATA,
  VIDEO_DATA
};

typedef enum msg_type_t;

struct P2P_PACKET {
  op_code_t          opcode;
  uint32_t         data_len;
  uint8_t            data[];
};

typedef struct P2P_PACKET p2p_packet_t;

int protocol_handshake(void) {
  return 0;
}
