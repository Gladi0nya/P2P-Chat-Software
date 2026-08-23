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

#include <stdio.h>

#include <unistd.h>

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
  addr_t addr;
  addr.port = 9999;
  
  if (stun_bind_sock(&sock, &addr)) {
    LOG_ERROR("stun_bind_sock() failed.");
    LOG_INFO("STUN method not available, other methods not implemented yet.");
    goto exit;
  }

  printf("Pub port : %u\n", addr.port);


  exit:
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");

  return log_shutdown();
}
