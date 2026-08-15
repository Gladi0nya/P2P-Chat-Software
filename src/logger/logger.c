/**
 * @file logger.c
 * @brief Logger module
 *
 * @author Tom Schmitt
 * @author Augustin Barniet
 *
 * Copyright (c) 2026 Tom Schmitt, Augustin Barniet
 * All rights reserved.
 *
 */

#include "logger.h"

#include <stdio.h>

#include <time.h>

static struct timespec start_time = {0};
static uint8_t isInit = 0;
static FILE* OUT = NULL;

/** ----------------------------------------------------------- *
  *  startLogger                                                *
  *                                                             *
  *  Initialize the logger module.                              *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

uint8_t startLogger(void)
{
  if (isInit)
    return 1;

  #ifdef LOGFILE
  if ((OUT = fopen(LOGFILE, "w+")) == NULL)
    return 1;
  #else
  OUT = stdout;
  #endif
  isInit = 1;
  
  return clock_gettime(CLOCK_MONOTONIC, &start_time) == -1;
}

/** ----------------------------------------------------------- *
  *  stoptLogger                                                *
  *                                                             *
  *  Stop the logger module.                                    *
  *                                                             *
  *  @retval 0 Successfully stopped.                            *
  *  @retval 1 Failed to stop.                                  *
  * ----------------------------------------------------------- **/

uint8_t stopLogger(void)
{
  if (!isInit)
    return 1;

  #ifdef LOGFILE
  if (fclose(OUT) == -1)
    return 1;
  #endif

  isInit = 0;
  return 0;
}

/** ----------------------------------------------------------- *
  *  getRunTime                                                 *
  *                                                             *
  *  Retrieve the elapsed time since the logger was initialized.*
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

static inline long double getRunTime(void)
{
  struct timespec current;

  clock_gettime(CLOCK_MONOTONIC, &current);

  const long double elapsed = (current.tv_sec - start_time.tv_sec) + 
          (current.tv_nsec - start_time.tv_nsec) / 1e9;

  return elapsed;
}

/** ----------------------------------------------------------- *
  *  logMsg                                                     *
  *                                                             *
  *  Display message in the logs.                               *
  *                                                             *
  *  @param code    Log level                                   *
  *  @param modname Module name                                 *
  *  @param msg     Message to be displayed                     * 
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

uint8_t logMsg(const uint8_t code, const char* modname, const char* msg)
{
  const char* LEVEL[] = {
    "ERROR  ",
    "WARNING",
    "INFO   ",
    "DEBUG  "
  };

  #ifndef LOGFILE
  const char* COLOR_LEVEL[] = {
    "\e[0;31m",
    "\e[0;33m",
    "\e[0;34m",
    "\e[0;32m"
  };
  #endif

  if (!isInit)
    return 1;

  #ifdef LOGFILE
  fprintf(OUT, "%Lf | %s | %s %s\n", getRunTime(), modname, LEVEL[code], msg);
  #else
  fprintf(OUT, "%Lf | %s | %s%s\e[0m - %s\n", getRunTime(), modname, COLOR_LEVEL[code], LEVEL[code], msg);
  #endif

  return 0;
}
