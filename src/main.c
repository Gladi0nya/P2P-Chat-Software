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

#include "core/bootstrap.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
  char* lport = NULL;
  char* rip = NULL;
  char* rport = NULL;
  
  for (int i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
      printf("Usage: %s -l [LISTENING_PORT] -r [PEER_IP] -p [PEER_PORT]\n", argv[0]);
      return 0;
    }

    if (strcmp(argv[i], "-l") == 0) {
      lport = argv[++i];
      continue;
    }

    if (strcmp(argv[i], "-r") == 0) {
      rip = argv[++i];
      continue;
    }

    if (strcmp(argv[i], "-p") == 0) {
      rport = argv[++i];
    }
  }
  
  return bootstrap_start(lport, rip, rport);
}
