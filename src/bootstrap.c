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

#include "logger.h"

#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

/** ----------------------------------------------------------- *
  *  bootstrap_run                                              *
  *                                                             *
  *  Entry point of the bootstrap module.                       *
  *                                                             *
  *  @param argc Number of program arguments                    *
  *  @param argv Program arguments                              *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

uint8_t bootstrap_run(void)
{
  if (log_init())
  {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }

  LOG_INFO("Bootstrap started.");

  net_init(9999);

  char peer_ip[16], peer_port[9];
  
  printf("Enter Peer IP: ");
  fflush(stdout);
  if (fgets(peer_ip, sizeof(peer_ip), stdin) != NULL) {
    peer_ip[strcspn(peer_ip, "\n")] = '\0';
  } else {
    LOG_ERROR("Input mismatch.");
    goto exit;
  }
  
  printf("Enter Peer PORT: ");
  fflush(stdout);
  if (fgets(peer_port, sizeof(peer_port), stdin) != NULL) {
    peer_port[strcspn(peer_port, "\n")] = '\0';
  } else {
    LOG_ERROR("Input mismatch.");
    goto exit;
  }

  net_create_chat(peer_ip, peer_port);
 
  exit:
  net_shutdown();
  
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");

  log_shutdown();
  
  return 0;
}
