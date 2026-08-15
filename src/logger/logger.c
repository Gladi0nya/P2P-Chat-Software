#include "logger.h"

#include <stdio.h>

#include <time.h>

static struct timespec start_time = {0};
static uint8_t isInit = 0;
static FILE* OUT = NULL;


uint8_t startLogger(void) {
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

uint8_t stopLogger(void) {
  if (!isInit)
    return 1;

  #ifdef LOGFILE
  if (fclose(OUT) == -1)
    return 1;
  #endif

  isInit = 0;
  return 0;
}

static inline long double getRunTime(void) {
  struct timespec current;

  clock_gettime(CLOCK_MONOTONIC, &current);

  const long double elapsed = (current.tv_sec - start_time.tv_sec) + 
          (current.tv_nsec - start_time.tv_nsec) / 1e9;

  return elapsed;
}

uint8_t logMsg(const uint8_t code, const char* modname, const char* msg) {
  const char* LEVEL[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG"
  };

  if (!isInit)
    return 1;
  
  fprintf(OUT, "[%Lf] [%s] [%s] - %s\n", getRunTime(), LEVEL[code], modname, msg);
  return 0;
}
