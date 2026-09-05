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
  int ret = 1;
  char peer_ip[16], peer_port[9], my_port[9];
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
  
    printf("What port would you like to listen on (>= 9999): ");
    fflush(stdout);
    
    if (fgets(my_port, sizeof(my_port), stdin) == NULL) {
      LOG_ERROR("Input mismatch.");
      goto exit;
    }
    
    my_port[strcspn(my_port, "\n")] = '\0'; 
  }

  if (peer_context_set_port(&ctx, lport))
    goto exit;

  LOG_INFO("Your Public Address is: %u.%u.%u.%u:%u",
	   (ctx.my_pub_ip      ) & 0xFF,
	   (ctx.my_pub_ip >>  8) & 0xFF,
	   (ctx.my_pub_ip >> 16) & 0xFF,
	   (ctx.my_pub_ip >> 24) & 0xFF,
	   ctx.my_port);


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

    printf("Enter Peer PORT: ");
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
  

  ret = node_run(&ctx);

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
