#ifndef NET_UDP_SOCKET_H
#define NET_UDP_SOCKET_H

#include "state/peer_state.h"

int udp_socket_create(int *sock);
int udp_socket_close(int *sock);

int udp_socket_open_port(int sock, const char *const restrict my_port,
			 uint16_t *const restrict out_port);

#endif
