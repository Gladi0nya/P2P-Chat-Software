#ifndef NODE_H
#define NODE_H
#include <stdint.h>

uint8_t CreateChannelForPeer(const char* peer_ip, const int peer_port, const int my_port);

#endif
