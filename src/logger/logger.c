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

#ifdef LOG_USE_DATETIME
  #include "string_acts.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>


//DEBUG             : Debug mode is declared as turned off

//LOG_NO_TERMINAL   : Not logging inside of the terminal

//LOG_USE_DATETIME  : Choosing the type of time used (date&time OR time from the begin of the program)
//                                                    defined   OR      not defined

//LOG_NAME_FILE     : Choosing the name of the log file


// Is logger initialized ? (1 = yes, 0 = not)
static uint8_t is_init = 0;

#ifdef LOG_USE_FILE
    // File used to make logs
    static FILE* restrict LOG_OUTPUT = NULL;
#else
    #ifdef LOG_NO_TERMINAL
        #undef LOG_NO_TERMINAL
    #endif
#endif

#ifdef LOG_USE_DATETIME
  // Size that should be used to format the time if using date and time type
  static uint8_t size_time = 0;

  /** ------------------------------------------------------------------------------------------- *
   *  log_date_and_time                                                                           *
   *                                                                                              *
   *  Retrieve and format the current date and time into a string                                 *
   *                                                                                              *
   *  @param formated     pointer of the char* for the formated time has to no be allocated       *
   *                      the length of the buffer has to be at least 20 (until year 9999)        *
   *  @param on_terminal  boolean to know whether the time will be printed on terminal            *
   *                      0 = not on terminal, 1 = on terminal                                    *
   *                                                                                              *
   *  @retval 0   pointer foration was done well                                                  *
   *  @retval 1   could not get the time                                                          *
   *  @retval 2   memory allocation failed                                                        *
   *  ------------------------------------------------------------------------------------------- **/

  uint8_t log_date_and_time(char* restrict const formated,const uint16_t on_terminal)
  {
      time_t now = time(NULL);
      struct tm* time = localtime(&now);
      if (time == NULL) {
          return 1;
      }
      char* converted[7] = {
          int_to_char((int32_t)time->tm_mday),
          int_to_char((int32_t)time->tm_mon + 1),
          int_to_char((int32_t)(time->tm_year + 1900)),
          int_to_char((int32_t)time->tm_hour),
          int_to_char((int32_t)time->tm_min),
          int_to_char((int32_t)time->tm_sec),
          NULL
      };
      uint8_t index = 0;
      if (size_time == 0) {
          size_time = 2 + 2 + length(converted[2]) + 2 + 2 + 2 + 1 * 6;
      }
      formated[size_time] = '\0';
      
      for (uint8_t index_converted = 0 ; converted[index_converted] != NULL ; index_converted++) {
          
          if (converted[index_converted] == NULL) {
              for (int k = 0 ; converted[k] != NULL ; k++) {
                  free(converted[k]);
              }

              return 2;

          } else if (converted[index_converted][1] == '\0') {
              formated[index] = '0';
              formated[index + 1] = converted[index_converted][0];

              index += 2;

          } else {
              for (uint32_t index_string = 0 ; converted[index_converted][index_string] != '\0' ; index_string++) {
                  formated[index] = converted[index_converted][index_string];

                  index++;
              }
          }

          switch(index_converted) {
              case 0:
              case 1:
                  formated[index] = '/';
                  break;
              
              case 3:
              case 4:
                  formated[index] = ':';
                  break;

              default:
                  if (on_terminal) {
                      formated[index] = ' ';
                  } else {
                      formated[index] = '\t';
                  }
          }
          free(converted[index_converted]);
          index++;
      }
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
  
  if (clock_gettime(CLOCK_MONOTONIC, &current) == -1) {
    return 1;
  }

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
  if (LOG_OUTPUT != NULL)
    return 1;
  
  if ((LOG_OUTPUT = fopen(NAME_FILE, "a+")) != NULL)
    return 1;
  #endif

  #ifndef LOG_USE_DATETIME
  if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0)
    return 1;
  #endif
        
  #ifdef DEBUG

  #ifndef LOG_NO_TERMINAL
  fprintf(stdout, "Log initialized.\n");
  #endif
  
  #ifdef LOG_USE_FILE
  fprintf(LOG_OUTPUT, "\tLog initialized.\n");
  #endif

  #endif

  is_init = 1;

  return 0;
}

/** ----------------------------------------------------------- *
  *  log_shutdown (You wrote log sutdown, at first, here my     *
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

  #ifndef LOG_NO_TERMINAL
  fprintf(stdout, "Log stopped.\n");
  #endif
  
  #ifdef LOG_USE_FILE
  fprintf(LOG_OUTPUT, "\tLog stopped.\n");
  #endif

  #endif

  #ifdef LOG_USE_FILE
  if (LOG_OUTPUT != NULL) {
    if (fclose(LOG_OUTPUT)){
      LOG_ERROR("Failed to close the log file");

      return 1;
    }
		
    LOG_OUTPUT = NULL;
  }
  #endif

  is_init = 0;

  return 0;
}


/** --------------------------------------------------------------- *
  *  log_write                                                      *
  *                                                                 *
  *  Display message in the logs.                                   *
  *                                                                 *
  *  @param level        Level of log                               *
  *  @param name_file    Name of the file which the log comes from  *
  *  @param name_func    Name of the function which needs a log     *
  *  @param line_number  Line where the log function was called     *
  *  @param msg          Message to print in the log                *
  *                                                                 *
  *  @retval 0 Successfully initialized.                            *
  *  @retval 1 Failed to initialized.                               *
  *  @retval 2 Not connected.                                       *
  * --------------------------------------------------------------- **/

uint8_t log_write(const uint8_t level, const char* restrict const filename,
		  const char* restrict const funcname, const uint64_t line,
      const char* restrict const format,...)
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
  char msg[2048];

  va_list arguments;
  va_start(arguments, format);
  vsnprintf(msg, sizeof(msg), format, arguments);
  va_end(arguments);

  #ifndef LOG_NO_TERMINAL
  const char* const restrict LEVEL_COLOR[] = {
    "\033[0;31m",
    "\033[0;33m",
    "\033[0;34m",
    "\033[0;32m"
  };
  #endif

  #ifdef LOG_USE_DATETIME
  char current_time[25];

  #ifndef LOG_NO_TERMINAL
  if (!(date_and_time(current_time,1)))
    fprintf(stdout,"%s | %s%s\033[0m | %s [%s:%s:%li]\n", current_time, LEVEL_COLOR[level], LEVEL_MSG[level], msg, filename, funcname, line);

  free(current_time);
  #endif

  #ifdef LOG_USE_FILE
  if (!(date_and_time(current_time,0))) {
    fprintf(LOGFILE,"%s\t%s\t%s\t%s\t%s\t%li\n", current_time, LEVEL_MSG[level], msg, filename, funcname, line);
    fflush(LOGFILE);
  }

  free(current_time);
  #endif
  
  #else
  
  float80_t elapsed;

  if (log_time_spent(&elapsed))
    return 1;

	    
  #ifndef LOG_NO_TERMINAL

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
