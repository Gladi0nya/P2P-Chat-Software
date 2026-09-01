#ifndef NET_UDP_SOCKET_H
#define NET_UDP_SOCKET_H

#include "state/peer_state.h"

int udp_socket_create(int *sock);
int udp_socket_close(int *sock);

int udp_socket_init_ctx(peer_context_t *ctx);
int udp_socket_open_port_ctx(peer_context_t *ctx, const char *my_port);
int udp_socket_set_peer_ctx(peer_context_t *ctx, const char *peer_ip, const char *my_port);
int udp_socket_close_ctx(peer_context_t *ctx);

#endif
