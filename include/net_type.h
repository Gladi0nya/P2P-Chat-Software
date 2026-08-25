#ifndef NET_TYPE_H
#define NET_TYPE_H

struct ADDR {
  uint32_t ip;
  uint16_t port;
};

typedef struct ADDR addr_t;
#endif
