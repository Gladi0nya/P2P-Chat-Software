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
#include "node.h"

#include <stdio.h>
#include <stdlib.h>

#define MODULE_NAME "BOOTSTRAP"

/** ----------------------------------------------------------- *
  *  main                                                       *
  *                                                             *
  *  Entry point of the bootstrap module.                       *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

int main(void)
{
  if (startLogger())
    return 1;
  
  logMsg(INFO, MODULE_NAME, "Logger initialized.");  

  char peer_ip[80];
  char peer_port[10];
  char my_port[10];

  printf("Enter peer IPv4 (x.x.x.x) :\n");
  fgets(peer_ip, sizeof(peer_ip), stdin);
  printf("Enter peer port :\n");
  fgets(peer_port, sizeof(peer_port), stdin);
  
  printf("Enter listening port :\n");
  fgets(my_port, sizeof(my_port), stdin);
  
  logMsg(INFO, MODULE_NAME, "Attempting Peer to Peer connection.");

  if (CreateChannelForPeer(peer_ip, atoi(peer_port), atoi(my_port))) {
    logMsg(ERROR, MODULE_NAME, "Failed to negotiate peer connection.");
    return 1;
  }

  logMsg(INFO, MODULE_NAME, "Exiting.");

  if (stopLogger())
    return 1;
  
  return 0;
}
