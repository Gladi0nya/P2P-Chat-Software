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

#include "bootstrap.h"
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

  LOG_INFO("Bootstrap started.");
  LOG_DEBUG("Logger initialized.");  

  if (stun_request()) {
    LOG_ERROR("stun_request() failed.");
    goto exit;
  }


  exit:
  LOG_INFO("Bootstrap exit.");
  LOG_DEBUG("Logger shutdown.");

  return log_shutdown();
}
