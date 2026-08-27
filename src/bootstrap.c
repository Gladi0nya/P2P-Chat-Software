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

int bootstrap_run(void)
{
  if (log_init())
  {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }

  LOG_INFO("Bootstrap started.");

  if (net_init())
  {
    LOG_ERROR("Failed to initialize net module.");
    goto exit;
  }

  char peer_ip[16], peer_port[9], my_port[9];

  printf("What port would you like to be listening to (Please provide higher than 1000, which are reserved): ");

  fflush(stdout);
  if (fgets(my_port, sizeof(my_port), stdin) != NULL) {
    peer_port[strcspn(my_port, "\n")] = '\0';
  } else {
    LOG_ERROR("Input mismatch.");
    goto exit;
  }

  net_connection_t net_conn;

  if (net_create(&net_conn)) {
    LOG_DEBUG("net_create() failed.");
    LOG_ERROR("Failed to create net handle.");

    goto exit;
  }
  
  if (net_open(&net_conn, my_port)) {
    LOG_DEBUG("net_open() failed.");
    LOG_ERROR("Failed to open port.");

    goto exit;
  }
  
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
  
  if (net_connect(&net_conn, peer_ip, peer_port))
    LOG_ERROR("Connection failed.");
  
 
  exit:
  
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");

  log_shutdown();
  
  return 0;
}
