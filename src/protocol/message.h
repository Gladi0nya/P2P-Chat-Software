#ifndef PROTOCOL_MESSAGE_H
#define PROTOCOL_MESSAGE_H

#include <stdint.h>

enum OPCODE {
  OP_PUNCH,
  OP_PUNCH_ACK,
  OP_PING,
  OP_PONG,
  OP_MSG,
  OP_CLOSE
};

typedef enum OPCODE opcode_t;

enum DATATYPE {
  DATA_CERTIFICATE,
  DATA_RAW,
  DATA_EXEC,
  DATA_UPDATE,
  DATA_MUSIC,
  DATA_AUDIO,
  DATA_VIDEO
};

typedef enum DATATYPE data_type_t;

struct NET_PACKET {
  opcode_t opcode;
  uint8_t data[];
};

typedef struct NET_PACKET net_packet_t;

struct P2P_PACKET {
  opcode_t opcode;
  data_type_t type;
  uint32_t datalen;
  uint8_t certificate[64];
  uint8_t data[];
};

typedef struct P2P_PACKET p2p_packet_t;

#endif
