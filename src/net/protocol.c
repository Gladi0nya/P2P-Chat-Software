#include "protocol.h"

enum DATA_TYPE {
  DATA_CERTIFICATE,
  DATA_RAW,
  DATA_EXEC,
  DATA_UPDATE,
  DATA_MUSIC,
  DATA_AUDIO,
  DATA_VIDEO
};

typedef enum data_type_t;

struct NET_PACKET {
  opcode_t opcode;
  void     data[];
};

typedef NET_PACKET net_packet_t;

struct P2P_PACKET {
  uint8_t     certificate[];
  op_code_t          opcode;
  uint32_t         data_len;
  uint8_t            data[];
};

typedef struct P2P_PACKET p2p_packet_t;

int protocol_handshake(void) {
  return 0;
}
