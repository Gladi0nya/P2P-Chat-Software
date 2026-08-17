/**
 * @file main.c
 * @brief Entry point
 *
 * @author Tom Schmitt
 * @author Augustin Barniet
 *
 * Copyright (c) 2026 Tom Schmitt, Augustin Barniet
 * All rights reserved.
 *
 */

#include "bootstrap.h"

/** ----------------------------------------------------------- *
  *  main                                                       *
  *                                                             *
  *  Entry point of the bootstrap module.                       *
  *                                                             *
  *  @param argc Number of program arguments                    *
  *  @param argv Program arguments                              *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

int main(int argc, char **argv)
{
  (void)argc; // Remove compiler warnings (might add program arguments later)
  (void)argv;
  
  return bootstrap_run();
}
