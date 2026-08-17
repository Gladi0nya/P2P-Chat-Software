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

#define MODULE_NAME "BOOTSTRAP"

uint8_t bootstrap_run(void)
{
  if (log_init())
  {
    fprintf(stderr, "Failed to initialize logger.\n");
    return 1;
  }
  
  log_msg(DEBUG, MODULE_NAME, "Logger initialized.");  

  log_msg(INFO,  MODULE_NAME, "Program exit.");

  log_msg(DEBUG, MODULE_NAME, "Logger shutdown.");

  return log_shutdown();
}
