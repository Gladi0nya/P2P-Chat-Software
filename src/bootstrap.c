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

#include <stdio.h>

uint8_t bootstrap_run(void)
{
  if (log_init())
  {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }
  
  LOG_DEBUG("Logger initialized.");  

  LOG_INFO("Program exit.");

  LOG_DEBUG("Logger shutdown.");

  return log_shutdown();
}
