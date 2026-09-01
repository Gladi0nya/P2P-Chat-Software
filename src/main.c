/**
 * @file main.c
 * @brief Entry point
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#include <stdint.h>

#include "core/bootstrap.h"

/** ----------------------------------------------------------- *
  *  main                                                       *
  *                                                             *
  *  Entry point of the bootstrap module.                       *
  *                                                             *
  *  @param argc Number of program arguments                    *
  *  @param argv Program arguments                              *
  *                                                             *
  *  @retval 0 Program ran without errors.                      *
  *  @retval 1 Program ran into an error.                       *
  * ----------------------------------------------------------- **/

int main(int argc, char **argv)
{
  // Remove compiler warnings (might add program arguments later)
  (void)argc; 
  (void)argv;
  
  return bootstrap_start();
}
