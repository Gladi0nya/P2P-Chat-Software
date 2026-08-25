#include "protocol.h"

enum PROTOCOL_OPCODE {
  HANDSHAKE,
  PEER_RENDEZVOUS,    // Open new connection for current peer
  FOREIGN_RENDEZVOUS, // Ask to be contacted by other peer
  KEEPALIVE,
  PING,
  PONG,
  DATA,
  DISCONNECT
};

enum PROTOCOL_MSG_TYPE {
  NODE_DISCOVERY,
  PARANOIA_ANTIHACK,
  MSG_DATA,
  MUSIC_DATA,
  AUDIO_DATA,
  VIDEO_DATA
}


struct P2P_PACKET {
  PROTOCOL_OPCODE   opcode;
  uint32_t        data_len;
  uint8_t*            data;
}

int protocol_handshake(void) {
  return 0;
}
