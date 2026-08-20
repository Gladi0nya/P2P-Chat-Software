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
static FILE* restrict OUT = NULL;

/** ----------------------------------------------------------- *
  *  log_init                                                   *
  *                                                             *
  *  Initialize the logger module.                              *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

uint8_t log_init(void)
{
  if (isInit) return 1;

  #ifdef LOGFILE
  if ((OUT = fopen(LOGFILE, "w+")) == NULL) return 1;
  #else
  OUT = stdout;
  #endif
  isInit = 1;
  
  return (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1);
}

/** ----------------------------------------------------------- *
  *  stop_logger                                                *
  *                                                             *
  *  Stop the logger module.                                    *
  *                                                             *
  *  @retval 0 Successfully stopped.                            *
  *  @retval 1 Failed to stop.                                  *
  * ----------------------------------------------------------- **/

uint8_t log_shutdown(void)
{
  if (!isInit) return 1;

  #ifdef LOGFILE
  if (fclose(OUT) == -1)
    return 1;
  #endif

  isInit = 0;
  return 0;
}

/** ----------------------------------------------------------- *
  *  log_time                                                   *
  *                                                             *
  *  Retrieve the elapsed time since the logger was initialized.*
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  * ----------------------------------------------------------- **/

static inline long double log_time(void)
{
  struct timespec current;

  clock_gettime(CLOCK_MONOTONIC, &current);

  const long double elapsed = (current.tv_sec - start_time.tv_sec) + 
          (current.tv_nsec - start_time.tv_nsec) / 1000000000.0L;

  return elapsed;
}

/** ----------------------------------------------------------- *
  *  log_write                                                  *
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

uint8_t log_write(const uint8_t level, const char* const restrict filename, const uint64_t line, const char* const restrict msg)
{
  const char* const restrict LEVEL_NAME[] = {
    "ERROR  ",
    "WARNING",
    "INFO   ",
    "DEBUG  "
  };

  #ifndef LOGFILE
  const char* const restrict LEVEL_COLOR[] = {
    "\e[0;31m",
    "\e[0;33m",
    "\e[0;34m",
    "\e[0;32m"
  };
  #endif

  if (!isInit) return 1;

  #ifdef LOGFILE
  fprintf(OUT, "%015.9Lf | %s:%lu | %s %s\n", (long double)log_time(), filename, line, LEVEL_NAME[level], msg);
  #else
  fprintf(OUT, "%015.9Lf | %s:%lu | %s%s\e[0m - %s\n", (long double)log_time(), filename, line, LEVEL_COLOR[level], LEVEL_NAME[level], msg);
  #endif

  return 0;
}
