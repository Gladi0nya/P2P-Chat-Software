#ifndef NET_H
#define NET_H

#include <stdint.h>

#include "net_type.h"

struct NET_CONNECTION {
  int sock;
  addr_t peer;
  uint16_t my_port;
};

typedef struct NET_CONNECTION net_connection_t;

int net_init(void);

int net_create(net_connection_t* const restrict conn);

int net_open(net_connection_t* conn, const char* my_port);

int net_connect(net_connection_t* conn, const char* peer_ip, const char* peer_port);

int net_close(net_connection_t* conn);

#endif
