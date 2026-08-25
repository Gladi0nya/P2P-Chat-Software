#include "net.h"
#include "net_helper.h"
#include "stun.h"
#include "node.h"

#include "logger.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <arpa/inet.h>

static uint32_t my_pub_addr;

int net_init(void) {
  if (stun_check(&my_pub_addr)) {
    LOG_DEBUG("stun_check() failed.");

    return 1;
  }

  LOG_INFO("Net module initialized.");
  
  return 0;
}

int net_create(net_connection_t* const restrict conn) {
  conn->sock      = -1;
  conn->my_port   = 0;
  conn->peer.ip   = 0;
  conn->peer.port = 0;

  return 0;
}

int net_open(net_connection_t* const restrict conn, const char* const restrict my_port) {
  if (net_socket_create(&conn->sock, SOCK_DGRAM)) {
    LOG_DEBUG("net_socket_create() failed.");

    return 1;
  }

  conn->my_port = (uint16_t)atoi(my_port);

  if (stun_bind_sock(&conn->sock, &conn->my_port)) {
    LOG_DEBUG("stun_bind_sock() failed.");

    return 1;
  }

  printf("Pub addr: %u.%u.%u.%u:%u\n", *(uint8_t*)&my_pub_addr, *((uint8_t*)&my_pub_addr + 1), *((uint8_t*)&my_pub_addr + 2), *((uint8_t*)&my_pub_addr + 3), conn->my_port); // Convert to logger function when format is available.
  
  return 0;
}

int net_connect(net_connection_t* const restrict conn, const char* const restrict peer_ip, const char* const restrict peer_port) {

  if (inet_pton(AF_INET, peer_ip, &conn->peer.ip) != 1) {
    LOG_ERROR("Failing to parse IP.");
    
    return 1;
  }

  conn->peer.port = (uint16_t)atoi(peer_port);
  
  if (CreateChannelForPeer(conn->sock, conn->peer) != 0)
    return 1;

  return 0;
}

int net_close(net_connection_t* conn) {
  net_socket_close(&conn->sock);

  return 0;
}
