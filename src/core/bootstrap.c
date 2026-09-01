/**
 * @file main.c
 * @brief Bootstrap module
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#include "bootstrap.h"
#include "node.h"
#include "net/udp_socket.h"
#include "net/stun_client.h"
#include "state/peer_registry.h"
#include "logger/logger.h"

#include <stdio.h>
#include <string.h>

/** ----------------------------------------------------------- *
  *  bootstrap_start                                            *
  *                                                             *
  *  Entry point of the bootstrap module.                       *
  *                                                             *
  *  @param argc Number of program arguments                    *
  *  @param argv Program arguments                              *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

int bootstrap_start(void)
{
  int ret = 1;
  peer_context_t ctx;
  char my_port[9], peer_ip[16], peer_port[9];
  
  if (log_init())
  {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }

  LOG_INFO("Bootstrap started.");

  udp_socket_init_ctx(&ctx);
  
  if (stun_client_check(&ctx.my_pub_ip)) {
    LOG_DEBUG("stun_client_check() failed.");
    goto exit;
  }

  printf("What port would you like to listen on (>= 9999): ");
  fflush(stdout);

  if (fgets(my_port, sizeof(my_port), stdin) == NULL) {
    LOG_ERROR("Input mismatch.");
    goto exit;
  }

  my_port[strcspn(my_port, "\n")] = '\0';

  if (udp_socket_open_port_ctx(&ctx, my_port)) {
    LOG_ERROR("udp_socket_open_port_ctx() failed.");
    goto exit;
  }

  LOG_INFO("Your Public Address is: %u.%u.%u.%u:%u",
	   (ctx.my_pub_ip      ) & 0xFF,
	   (ctx.my_pub_ip >>  8) & 0xFF,
	   (ctx.my_pub_ip >> 16) & 0xFF,
	   (ctx.my_pub_ip >> 24) & 0xFF,
	   ctx.my_port);
  
  printf("Enter Peer IP: ");
  fflush(stdout);
  if (fgets(peer_ip, sizeof(peer_ip), stdin) == NULL) {
    LOG_ERROR("Input mismatch.");
    goto close_sock;
  }
  peer_ip[strcspn(peer_ip, "\n")] = '\0';

  printf("Enter Peer PORT: ");
  fflush(stdout);
  if (fgets(peer_port, sizeof(peer_port), stdin) == NULL) {
    LOG_ERROR("Input mismatch.");
    goto close_sock;
  }
  peer_port[strcspn(peer_port, "\n")] = '\0';

  if (udp_socket_set_peer_ctx(&ctx, peer_ip, peer_port)) {
    LOG_ERROR("udp_socket_set_peer() failed.");
    goto close_sock;
  }

  if (peer_registry_add(&ctx)) {
    LOG_ERROR("peer_registry_add() failed.");
    goto close_sock;
  }

  node_run(&ctx); 

  ret = 0;

 close_sock:
  udp_socket_close_ctx(&ctx);
  
 exit:
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");
  log_shutdown();
  
  return ret;
}
