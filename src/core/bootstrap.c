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

#ifdef _WIN32
static WSADATA wsaData = {0};
#endif

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

int bootstrap_start(char* lport, char* rip, char* rport)
{
  int ret = 1, is_peer_sym = 0, net_state;
  char peer_ip[16], peer_port[9], my_port[9], sym_input[3];
  peer_context_t ctx;
  
  if (log_init()) {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }

  LOG_INFO("Bootstrap started.");

  if (peer_context_create(&ctx))
    goto exit;

  if (lport == NULL) {
    lport = my_port;
  
    printf("Enter listening port (val >= 1024): ");
    fflush(stdout);
    
    if (fgets(my_port, sizeof(my_port), stdin) == NULL) {
      LOG_ERROR("Input mismatch.");
      goto exit;
    }
    
    my_port[strcspn(my_port, "\n")] = '\0'; 
  }

  if ((net_state = peer_context_check_net_state(&ctx)) == 1)
    goto exit;
  
  
  if (peer_context_set_port(&ctx, lport))
    goto exit;

  if (net_state)
    LOG_INFO("Your Public Address is: %u.%u.%u.%u, lowest_port: %u",
	   (ctx.my_pub_ip      ) & 0xFF,
	   (ctx.my_pub_ip >>  8) & 0xFF,
	   (ctx.my_pub_ip >> 16) & 0xFF,
	   (ctx.my_pub_ip >> 24) & 0xFF,
	   (uint16_t)net_state);
  else
    LOG_INFO("Your Public Address is: %u.%u.%u.%u:%u",
	   (ctx.my_pub_ip      ) & 0xFF,
	   (ctx.my_pub_ip >>  8) & 0xFF,
	   (ctx.my_pub_ip >> 16) & 0xFF,
	   (ctx.my_pub_ip >> 24) & 0xFF,
	   ctx.my_port);

  printf("Is your peer symmetric? (Y/n) ");
  fflush(stdout);

  if (fgets(sym_input, sizeof(sym_input), stdin) == NULL) {
    LOG_ERROR("Input mismatch.");
    goto close_sock;
  }

  if (sym_input[0] == 'y' || sym_input[0] == 'Y')
    is_peer_sym = 1;

  if (is_peer_sym && net_state) {
    LOG_ERROR("Inter NAT-symmetric negotiation not yet implemented.");
    goto close_sock;
  }
  
  if (rip == NULL) {
    rip = peer_ip;
    
    printf("Enter Peer IP: ");
    fflush(stdout);
    
    if (fgets(peer_ip, sizeof(peer_ip), stdin) == NULL) {
      LOG_ERROR("Input mismatch.");
      goto close_sock;
    }

    peer_ip[strcspn(peer_ip, "\n")] = '\0';
  }

  if (rport == NULL) {
    rport = peer_port;

    if (is_peer_sym) printf("Enter Lowest Peer PORT:"); // Defaulting Remote PORT (Symmetric needs bruteforce)
    else printf("Enter Peer PORT: ");
    
    fflush(stdout);
    if (fgets(peer_port, sizeof(peer_port), stdin) == NULL) {
      LOG_ERROR("Input mismatch.");
      goto close_sock;
    }

    peer_port[strcspn(peer_port, "\n")] = '\0';
  }
  
  if (peer_context_set_remote(&ctx, rip, rport))
    goto close_sock;
  

  if (peer_registry_add(&ctx))
    goto close_sock;
  

  ret = node_run(&ctx, is_peer_sym);

 close_sock:
  peer_context_close(&ctx);

  #ifdef _WIN32
  WSACleanup();
  #endif
  
 exit:
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");
  log_shutdown();
  
  return ret;
}
