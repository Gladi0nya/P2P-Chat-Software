#include "net.h"
#include "stun.h"
#include "node.h"

#include "logger.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <arpa/inet.h>

static int sock;
static addr_t my_addr;

int net_init(uint16_t port_chosen) {
  LOG_INFO("Net module started.");
  
  my_addr.port = port_chosen;
  
  if (stun_bind_sock(&sock, &my_addr)) {
    LOG_DEBUG("stun_bind_sock() failed.");
    LOG_INFO("STUN method not available, other methods not implemented yet.");

    return 1;
  }
  
  LOG_INFO("Net module initialized.");

  printf("Pub ip %u.%u.%u.%u:%u\n", *(uint8_t*)&my_addr.ip, *((uint8_t*)&my_addr.ip + 1), *((uint8_t*)&my_addr.ip + 2), *((uint8_t*)&my_addr.ip + 3), my_addr.port); // Convert to logger function when format is available.
  
  return 0;
}

int net_create_chat(char* peer_ip, char* peer_port) {
  addr_t peer_addr;

  if (inet_pton(AF_INET, peer_ip, &peer_addr) != 1) {
    LOG_ERROR("Failing to parse IP.");
    
    return 1;
  }

  peer_addr.port = (uint16_t)atoi(peer_port);
  
  if (CreateChannelForPeer(sock, peer_addr) != 0)
    return 1;

  return 0;
}

int net_shutdown(void) {
  LOG_INFO("Net module shutdown.");
  
  close(sock);

  return 0;
}
