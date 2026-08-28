/**
 * @file logger.c
 * @brief Logger module
 *
 * @author Augustin Barniet
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Augustin Barniet, Tom Schmitt
 * All rights reserved.
 *
 */

#include "logger.h"
#include "generic_type.h"
// #include "string_acts.h" Not used so I removed it

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

// Is logger initialized ? (1 = yes, 0 = not)
static uint8_t is_init = 0;

#ifdef LOG_USE_FILE
    // File used to make logs
    static FILE* restrict OUT = NULL;
#else
    #ifdef LOG_USE_AUGUSTIN
        #undef LOG_USE_AUGUSTIN
    #endif
#endif

#ifdef LOG_USE_DATETIME
// Size that should be used to format the time if using date and time type
static uint8_t size_time = 0;

/** ------------------------------------------------------------------------------------------- *
 *  date_and_time                                                                               *
 *                                                                                              *
 *  Retrieve and format the current date and time into a string                                 *
 *                                                                                              *
 *  @param formated     pointer of the char* for the formated time has to no be allocated       *
 *  @param on_terminal  boolean to know whether the time will be printed on terminal            *
 *                      0 = not on terminal                                                     *
 *                                                                                              *
 *  @retval 0   pointer foration was done well                                                  *
 *  @retval 1   could not get the time                                                          *
 *  @retval 2   memory allocation failed                                                        *
 *  ------------------------------------------------------------------------------------------- **/

uint8_t date_and_time(char** restrict formated, const uint16_t on_terminal) // TODO: This function looks kinda fatty, need of reducing its size and optimizing it.
{
  uint8_t index = 0;
  time_t now = time(NULL);
  struct tm* time = localtime(&now);
  
  if (time == NULL)
    return 1;
  
  char* converted[7] = {
    int_to_char((int32_t)time->tm_mday),
    int_to_char((int32_t)time->tm_mon + 1),
    int_to_char((int32_t)(time->tm_year + 1900)),
    int_to_char((int32_t)time->tm_hour),
    int_to_char((int32_t)time->tm_min),
    int_to_char((int32_t)time->tm_sec),
    NULL
  };
  
  if (size_time == 0)
    size_time = 2 + 2 + length(converted[2]) + 2 + 2 + 2 + 1 * 6;
  
  
  (*formated) = malloc(sizeof(char) * (size_time + 1));
  
  if ((*formated) == NULL) {

    for (uint8_t k = 0 ; converted[k] != NULL ; k++)
      free(converted[k]);
    
    return 2;
  }
  
  for (uint8_t index_converted = 0 ; converted[index_converted] != NULL ; index_converted++) {

    if (converted[index_converted] == NULL) {

      for (int k = 0 ; converted[k] != NULL ; k++)
	free(converted[k]);
          
      free(*formated);

      return 2;

    } else if (converted[index_converted][1] == '\0') {
      
      (*formated)[index] = '0';
      (*formated)[index + 1] = converted[index_converted][0];

      index += 2;
      
    } else {

      for (uint32_t index_string = 0 ; converted[index_converted][index_string] != '\0' ; index_string++) {
	(*formated)[index] = converted[index_converted][index_string];
	
	index++;
      }
    }

    switch(index_converted) {
        case 1:
	  (*formated)[index] = '/';
	  break;
        case 4:
	  (*formated)[index] = ':';
	  break;
        default:
	  if (on_terminal) {
	    (*formated)[index] = ' ';
	  } else {
	    (*formated)[index] = '\t';
	  }
    }

    free(converted[index_converted]);

    index++;
  }

  (*formated)[size_time - 1] = '\0';

  return 0;
}
#else
// Time of the begin of the program
static struct timespec start_time = {0};

/** ------------------------------------------------------------------------------------------- *
 *  log_time_spent                                                                              *
 *                                                                                              *
 *  Retrieve the elapsed time since the logger was initialized                                  *
 *                                                                                              *
 *  @retval 0   time passed was assigned to elapsed                                             *
 *  @retval 1   error                                                                           *
 *  ------------------------------------------------------------------------------------------- **/

static inline uint8_t log_time_spent(float80_t* elapsed)
{
  struct timespec current;
  
  if (clock_gettime(CLOCK_MONOTONIC, &current) == -1)
    return 1;
  

  *elapsed = (current.tv_sec - start_time.tv_sec) +
    (current.tv_nsec - start_time.tv_nsec) / 1000000000.0L;

  return 0;
}
#endif


/** ----------------------------------------------------------- *
  *  log_init                                                   *
  *                                                             *
  *  Initialize the logger module.                              *
  *                                                             *
  *  @retval 0 Successfully initialized.                        *
  *  @retval 1 Failed to initialized.                           *
  *  @retval 2 Already connected.                               *
  * ----------------------------------------------------------- **/

uint8_t log_init(void)
{
  if (is_init)
    return 2;

  #ifdef LOG_USE_FILE
  if (OUT != NULL)
    return 1;
  
  if ((OUT = fopen(NAME_FILE, "a+")) != NULL)
    return 1;
  #endif

  #ifndef LOG_USE_DATETIME
  if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0)
    return 1;
  #endif
        
  #ifdef DEBUG

  #ifndef LOG_USE_AUGUSTIN
  fprintf(stdout, "Log initialized.\n");
  #endif
  
  #ifdef LOG_USE_FILE
  fprintf(OUT, "\tLog initialized.\n");
  #endif

  #endif

  is_init = 1;

  return 0;
}

/** ----------------------------------------------------------- *
  *  log_shitdown (You wrote log sutdown, at first, here my     *
  *                answer)                                      *
  *                                                             *
  *  Stop the logger module.                                    *
  *                                                             *
  *  @retval 0 Successfully stopped.                            *
  *  @retval 1 Failed to stop.                                  *
  *  @retval 2 Not connected.                                   *
  * ----------------------------------------------------------- **/

uint8_t log_shutdown(void) {

  if (!is_init)
    return 2;

  #ifdef DEBUG

  #ifndef LOG_USE_AUGUSTIN
  fprintf(stdout, "Log stopped.\n");
  #endif
  
  #ifdef LOG_USE_FILE
  fprintf(OUT, "\tLog stopped.\n");
  #endif

  #endif

  #ifdef LOG_USE_FILE
  if (OUT != NULL) {
    if (fclose(OUT)){
      LOG_ERROR("Failed to close the log file");

      return 1;
    }
		
    OUT = NULL;
  }
  #endif

  is_init = 0;

  return 0;
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
  *  @retval 2 Not connected.                                   *
  * ----------------------------------------------------------- **/

uint8_t log_write(const uint8_t level, const char* restrict const filename,
		  const char* restrict const funcname, const uint64_t line, const char* restrict const msg)
{
  
  if (!is_init)
    return 1;

  #ifndef DEBUG // TODO: Change behaviour, must be stay static (only at compile-time)
  if (level == LOG_LEVEL_DEBUG) 
    return 3;
  #endif

  const char* const restrict LEVEL_MSG[] = {
    "ERROR",
    "WARN ",
    "INFO ",
    "DEBUG"
  };

  #ifndef LOG_USE_AUGUSTIN
  const char* const restrict LEVEL_COLOR[] = {
    "\033[0;31m",
    "\033[0;33m",
    "\033[0;34m",
    "\033[0;32m"
  };
  #endif

  #ifdef LOG_USE_DATETIME
  char* const restrict current_time;

  #ifndef LOG_USE_AUGUSTIN
  if (!(date_and_time((char** restrict)&current_time,1)))
    fprintf(stdout,"%s | %s%s\033[0m | %s [%s:%s:%li]\n", current_time, LEVEL_COLOR[level], LEVEL_MSG[level], msg, filename, funcname, line);

  free(current_time);
  #endif

  #ifdef LOG_USE_FILE
  if ((date_and_time((char** restrict)&current_time,0))) {

    fprintf(LOGFILE,"%s\t%s\t%s\t%s\t%s\t%li\n", current_time, LEVEL_MSG[level], msg, filename, funcname, line);
    fflush(LOGFILE);
  }

  free(current_time);
  #endif
  
  #else
  
  float80_t elapsed;

  if (log_time_spent(&elapsed))
    return 1;

	    
  #ifndef LOG_USE_AUGUSTIN

  #ifdef __x86_64__
  fprintf(stdout,"%015.9Lf | %s%s\033[0m | %s [%s:%s:%li]\n", elapsed, LEVEL_COLOR[level], LEVEL_MSG[level], msg, filename, funcname, line);
  #else
  fprintf(stdout,"%015.9Lf | %s%s\033[0m | %s [%s:%s:%lli]\n", elapsed, LEVEL_COLOR[level], LEVEL_MSG[level], msg, filename, funcname, line);
  #endif

  #endif
  
  #ifdef LOG_USE_FILE
  fprintf(LOGFILE,"%015.9Lf\t%s\t%s\t%s\t%s\t%lli\n", elapsed, LEVEL_MSG[level], msg, filename, funcname, line);
  fflush(LOGFILE);
  #endif

  #endif

  return 0;
}
