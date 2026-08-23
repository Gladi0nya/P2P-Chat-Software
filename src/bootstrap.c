/**
 * @file main.c
 * @brief Bootstrap module
 *
 * @author Tom Schmitt
 * @author Augustin Barniet
 *
 * Copyright (c) 2026 Tom Schmitt, Augustin Barniet
 * All rights reserved.
 *
 */

#include "logger.h"
#include "stun.h"
#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

uint8_t bootstrap_run(void)
{
  if (log_init())
  {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }

  LOG_DEBUG("Logger initialized.");  
  LOG_INFO("Bootstrap started.");

  int sock;
  addr_t my_addr, peer_addr;
  my_addr.port = 9999;
  
  if (stun_bind_sock(&sock, &my_addr)) {
    LOG_ERROR("stun_bind_sock() failed.");
    LOG_INFO("STUN method not available, other methods not implemented yet.");
    goto exit;
  }

  printf("Pub ip %u.%u.%u.%u:%u\n", *(uint8_t*)&my_addr.ip, *((uint8_t*)&my_addr.ip + 1), *((uint8_t*)&my_addr.ip + 2), *((uint8_t*)&my_addr.ip + 3), my_addr.port);

  char input[256];
  
  printf("Enter Peer IP: ");
  if (fgets(input, sizeof(input), stdin) != NULL) {
    input[strcspn(input, "\n")] = '\0';
  } else {
    LOG_ERROR("Input mismatch.");
    goto exit;
  }

  if (inet_pton(AF_INET, input, &peer_addr) != 1) {
    LOG_ERROR("Invalid IP.");
    
    goto exit;
  }
  
  printf("Enter Peer PORT: ");
  if (fgets(input, sizeof(input), stdin) != NULL) {
    input[strcspn(input, "\n")] = '\0';
  } else {
    LOG_ERROR("Input mismatch.");
    goto exit;
  }

  peer_addr.port = (uint16_t)atoi(input);

  CreateChannelForPeer(sock, peer_addr);

  exit:
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");

  return log_shutdown();
}
