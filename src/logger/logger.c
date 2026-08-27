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
#include "generic_type.h"
#include "string_acts.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

//Debug mode is declared as turned off
//#define DEBUG_MOD

//Not logging inside of the terminal
//#define NO_TERMINAL

//Choosing the type of time used (date&time OR time from the begin of the program)
//#define TYPE_TIME         //      defined   OR      not defined

//Choosing the name of the log file
//#define NAME_FILE


//Logger initialized or not (0 = not)
static int8_t is_init = 0;

#ifdef NAME_FILE
    //File used to make logs
    static FILE* restrict OUT = NULL;
#else
    #ifdef NO_TERMINAL
        #undef NO_TERMINAL
    #endif
#endif

#ifndef TYPE_TIME
    //Time of the begin of the program
    static struct timespec start_time = {0};

    /** ------------------------------------------------------------------------------------------- *
     *  time_lasts                                                                                  *
     *                                                                                              *
     *  Retrieve the elapsed time since the logger was initialized                                  *
     *                                                                                              *
     *  @retval 0   time passed was assigned to elapsed                                             *
     *  @retval 1   error                                                                           *
     *  ------------------------------------------------------------------------------------------- **/

    static inline uint8_t time_lasts(float80_t* elapsed)
    {
        struct timespec current;
        if (clock_gettime(CLOCK_MONOTONIC, &current) == -1)
        {
            return 1;
        }

        *elapsed = (current.tv_sec - start_time.tv_sec) + (current.tv_nsec - start_time.tv_nsec) / 1000000000.0L;
        return 0;
    }


    /** ------------------------------------------------------------------------------------------- *
     *  write_time_lasts                                                                            *
     *                                                                                              *
     *  Write the logs when the lasting time data is needed                                         *
     *                                                                                              *
     *  @param output       output where the log has to be written                                  *
     *  @param level        level of log                                                            *
     *  @param level_color  tab color used to write the level of log inside of the terminal         *
     *                      NULL when writing inside of a log file                                  *
     *  @param level_msg    convertion from the integer : level, to a char* ready to print          *
     *  @param name_file    name of the file which called for a log                                 *
     *  @param name_func    name of the function which called for a log                             *
     *  @param line_number  number of the line which called for a log                               *
     *  @param msg          message to print inside of the log                                      *
     *                                                                                              *
     *  @retval 0   wrote the given message inside of the log                                       *
     *  @retval 1   logs are not initialized                                                        *
     *  @retval 2   nothing could be printed                                                        *
     *  ------------------------------------------------------------------------------------------- **/

    uint8_t write_time_lasts(const uint8_t level, const char* restrict const level_color[], const char* restrict const level_msg[], const char* restrict const name_file, const char* restrict const name_func, const uint64_t line_number, const char* restrict const msg)
    {
        if (is_init)
        {
            #ifdef NO_TERMINAL
                (void)level_color;
            #endif
            uint8_t done = 2;

            float80_t current_lasts;
            if (!(time_lasts(&current_lasts)))
            {
                #ifndef NO_TERMINAL
                    #ifdef DEBUG_MOD
                        fprintf(stdout, "%015.9Lf | %s%s\033[0m | %s [%s:%s:%li]\n", current_lasts, level_color[level], level_msg[level], msg, name_file, name_func, line_number);
                    #else
                        fprintf(stdout, "%015.9Lf | %s%s\033[0m | %s\n", current_lasts, level_color[level], level_msg[level], msg);
                    #endif
                    done = 0;
                #endif
                
                #ifdef NAME_FILE
                    #ifdef DEBUG_MOD
                        fprintf(LOGFILE, "%015.9Lf\t%s\t%s\t%s\t%s\t%li\n", current_lasts, level_msg[level], msg, name_file, name_func, line_number);
                    #else
                        fprintf(LOGFILE, "%015.9Lf\t%s\t%s\n", current_lasts, level_msg[level], msg);
                    #endif
                    fflush(LOGFILE);
                    done = 0;
                #endif
            }

            return done;
        }
        return 1;
    }


#else
    //Size that should be used to format the time if using date and time type
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

    uint8_t date_and_time(char** restrict formated, const uint16_t on_terminal)
    {
        time_t now = time(NULL);
        struct tm* time = localtime(&now);
        if (time == NULL)
        {
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
        if (size_time == 0)
        {
            size_time = 2 + 2 + length(converted[2]) + 2 + 2 + 2 + 1 * 6;
        }
        (*formated) = malloc(sizeof(char) * (size_time + 1));
        if ((*formated) == NULL)
        {
            for (uint8_t k = 0 ; converted[k] != NULL ; k++)
            {
                free(converted[k]);
            }
            return 2;
        }
        
        for (uint8_t index_converted = 0 ; converted[index_converted] != NULL ; index_converted++)
        {
            if (converted[index_converted] == NULL)
            {
                for (int k = 0 ; converted[k] != NULL ; k++)
                {
                    free(converted[k]);
                }
                free(*formated);
                return 2;
            }
            else if (converted[index_converted][1] == '\0')
            {
                (*formated)[index] = '0';
                (*formated)[index + 1] = converted[index_converted][0];
                index += 2;
            }
            else
            {
                for (uint32_t index_string = 0 ; converted[index_converted][index_string] != '\0' ; index_string++)
                {
                    (*formated)[index] = converted[index_converted][index_string];
                    index++;
                }
            }
            switch(index_converted)
            {
                case 0:
                case 1:
                    (*formated)[index] = '/';
                    break;
                
                case 3:
                case 4:
                    (*formated)[index] = ':';
                    break;

                default:
                    if (on_terminal)
                    {
                        (*formated)[index] = ' ';
                    }
                    else
                    {
                        (*formated)[index] = '\t';
                    }
            }
            free(converted[index_converted]);
            index++;
        }
        (*formated)[size_time - 1] = '\0';
        return 0;
    }


    /** ------------------------------------------------------------------------------------------- *
     *  write_date_time                                                                             *
     *                                                                                              *
     *  Write the logs when the whole date data is needed                                           *
     *                                                                                              *
     *  @param level        level of log                                                            *
     *  @param level_color  tab color used to write the level of log inside of the terminal         *
     *                      NULL when writing inside of a log file                                  *
     *  @param level_msg    convertion from the integer : level, to a char* ready to print          *
     *  @param name_file    name of the file which called for a log                                 *
     *  @param name_func    name of the function which called for a log                             *
     *  @param line_number  number of the line which called for a log                               *
     *  @param msg          message to print inside of the log                                      *
     *                                                                                              *
     *  @retval 0   wrote the given message inside of the log                                       *
     *  @retval 1   logs are not initialized                                                        *
     *  @retval 2   nothing could be printed                                                        *
     *  ------------------------------------------------------------------------------------------- **/

    uint8_t write_date_time(const uint8_t level, const char* restrict const level_color[], const char* restrict const level_msg[], const char* restrict const name_file, const char* restrict const name_func, const uint64_t line_number, const char* restrict const msg)
    {
        if (is_init)
        {
            #ifdef NO_TERMINAL
                (void)level_color;
            #endif
            uint8_t done = 2;

            char* restrict current_time;

            #ifndef NO_TERMINAL
                if (!(date_and_time((char** restrict)&current_time, 1)))
                {
                    #ifdef DEBUG_MOD
                        fprintf(stdout, "%s | %s%s\033[0m | %s [%s:%s:%li]\n", current_time, level_color[level], level_msg[level], msg, name_file, name_func, line_number);
                    #else
                        fprintf(stdout, "%s | %s%s\033[0m | %s\n", current_time, level_color[level], level_msg[level], msg);
                    #endif
                    done = 0;
                }
                free(current_time);
            #endif

            #ifdef NAME_FILE
                if ((date_and_time((char** restrict)&current_time, 0)))
                {
                    #ifdef DEBUG_MOD
                        fprintf(LOGFILE, "%s\t%s\t%s\t%s\t%s\t%li\n", current_time, level_msg[level], msg, name_file, name_func, line_number);
                    #else
                        fprintf(LOGFILE, "%s\t%s\t%s\n", current_time, level_msg[level], msg);
                    #endif
                    fflush(LOGFILE);
                    done = 0;
                }
                free(current_time);
            #endif
            
            return done;
        }
        return 1;
    }

#endif

#ifdef DEBUG_MOD
    /** ------------------------------------------------------------------------------------------- *
     *  write_connect                                                                               *
     *                                                                                              *
     *  Write a connection/disconnection message as a log                                           *
     *                                                                                              *
     *  @param msg     Message to print in the terminal as a log                                    *
     *                                                                                              *
     *  @retval 0   wrote the given message inside of the log                                       *
     *  @retval 1   logs are not initialized                                                        *
     *  @retval 2   nothing could be printed                                                        *
     *  ------------------------------------------------------------------------------------------- **/

    uint8_t write_connect(const char* restrict const msg)
    {
        #ifndef NO_TERMINAL
            fprintf(stdout, "%s\n", msg);
        #endif

        #ifdef NAME_FILE
            fprintf(OUT, "\t%s\n", msg);
        #endif

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
    if (!(is_init))
    {
        uint8_t done = 0;

        #ifdef NAME_FILE
            if (OUT == NULL)
            {
                if ((OUT = fopen(NAME_FILE, "a+")) == NULL){
                    return (uint8_t)0;
                }
                done = 1;
            }
        #endif

        #ifndef TYPE_TIME
            done = (!((uint8_t)(clock_gettime(CLOCK_MONOTONIC, &start_time) == 0)));
        #endif
        
        #ifdef DEBUG_MOD
            if (write_connect("Logs started"))
            {
                //Add something to print in case no message wase printed??
            }
            done = 0;
        #endif

        #ifndef NO_TERMINAL
            is_init = (int8_t)1;
        #endif
        #ifdef NAME_FILE
            is_init = (int8_t)1;
        #endif

        return done;
    }
    return (uint8_t)2;
}

/** ----------------------------------------------------------- *
  *  log_sutdown                                                *
  *                                                             *
  *  Stop the logger module.                                    *
  *                                                             *
  *  @retval 0 Successfully stopped.                            *
  *  @retval 1 Failed to stop.                                  *
  *  @retval 2 Not connected.                                   *
  * ----------------------------------------------------------- **/

uint8_t log_shutdown(void)
{
    if (is_init)
    {
        #ifdef DEBUG_MOD
            if (!(write_connect("Logs stopped")))
            {
                //Add something to print in case no message wase printed??
            }
        #endif

        #ifdef NAME_FILE
            if (OUT != NULL)
            {
                if (fclose(OUT))
                {
                    LOG_ERROR("Failed to close the log file");
                    return (uint8_t)1;
                }
                OUT = NULL;
            }
        #endif

        is_init = (int8_t)0;
        return (uint8_t)0;
    }
    return (uint8_t)2;
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

uint8_t log_write(const uint8_t level, const char* restrict const filename, const char* restrict const funcname, const uint64_t line, const char* restrict const msg)
{
    if (is_init != (int8_t)0)
    {
        #ifndef DEBUG_MOD
            if (level == LOG_LEVEL_DEBUG)
            {
                return (int8_t)3;
            }
        #endif

        const char* const restrict LEVEL_MSG[] = {
            "ERROR",
            "WARN ",
            "DEBUG",
            "INFO "
        };

        int8_t done_printing = 1;

        #ifndef NO_TERMINAL
            const char* const restrict LEVEL_COLOR[] = {
                "\033[0;31m",
                "\033[0;33m",
                "\033[0;32m",
                "\033[0;34m"
            };
        #else
            const char* const restrict LEVEL_COLOR[] = {NULL};
        #endif

        #ifndef TYPE_TIME
            done_printing = write_time_lasts(level, LEVEL_COLOR, LEVEL_MSG, filename, funcname, line, msg);
        #else
            done_printing = write_date_time(level, LEVEL_COLOR, LEVEL_MSG, filename, funcname, line, msg);
        #endif

        if (!(done_printing))
        {
            return (uint8_t)0;
        }
        return (uint8_t)1;
    }
    return (uint8_t)2;
}
