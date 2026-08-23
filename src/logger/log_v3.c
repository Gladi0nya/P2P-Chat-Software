#include "workingonit.h"

#include <stdlib.h>
#include <time.h>


#define String
#define Pointer
//#define FloatType
#define File
#include "../usefull_functions.h"


//Debug mode is declared as turned off
//#define DEBUG_MOD
//Not logging inside of the terminal
//#define NO_TERMINAL
//Choosing the type of time used (date&time OR time from the begin of the program)
//#define TYPE_TIME         //      defined   OR      not defined


typedef long double float80_t;

//File used to make logs
static FILE* restrict LOGFILE = NULL;
//Logger initialized or not (0 = not)
static int8_t is_init = 0;

#ifndef TYPE_TIME
//Time of the begin of the program
static struct timespec start_time = {0};

/** ------------------------------------------------------------------------------------------- *
 *  time_lasts                                                                                  *
 *                                                                                              *
 *  Retrieve the elapsed time since the logger was initialized                                  *
 *                                                                                              *
 *  @retval elapsed   Amount of seconds that have passed since the log has started              *
 *  ------------------------------------------------------------------------------------------- **/

static inline float80_t time_lasts(void)
{
    struct timespec current;
    if (clock_gettime(CLOCK_MONOTONIC,&current) == -1){
        return 0;
    }

    const float80_t elapsed = (current.tv_sec - start_time.tv_sec) + (current.tv_nsec - start_time.tv_nsec) / 1000000000.0L;
    return elapsed;
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
 *  @retval 0       error                                                                       *
 *  @retval done    wrote the given message inside of the log                                   *
 *  ------------------------------------------------------------------------------------------- **/

int8_t write_time_lasts(FILE* restrict const output,const uint8_t level,const char* restrict const level_color[],const char* restrict const level_msg[],const char* restrict const name_file,const char* restrict const name_func,const uint64_t line_number,const char* restrict const msg)
{
    uint8_t done = 0;

    if (level != LEVEL_FOL)
    {
        float80_t current_lasts = time_lasts();
        if (current_lasts != (float80_t)0)
        {
            if (output == stdout)
            {
                fprintf(output,"%015.9Lf | %s%s\e[0m | %s [%s - %s:%li]\n",(float80_t)time_lasts(),level_color[level],level_msg[level],msg,name_file,name_func,line_number);
                done++;
            }
            else if (level_color == NULL)
            {
                fprintf(output,"%015.9Lf\t%s\t%s\t%s\t%s\t%li\n",(float80_t)time_lasts(),level_msg[level],msg,name_file,name_func,line_number);
                done++;
            }
            //Else, do nothing
        }
    }
    else
    {
        if (output == stdout)
        {
            char replace_time[16];
            for (uint8_t k = 0 ; k < 16 ; k++)
            {
                replace_time[k] = ' ';
            }
            replace_time[15] = '\0';

            fprintf(output,"%s | %s%s\e[0m | %s [%s - %s:%li]\n",replace_time,level_color[level],level_msg[level],msg,name_file,name_func,line_number);
            done++;
        }
        else if (level_color == NULL)
        {
            fprintf(output,"\t\t%s\t%s\t%s\t%s\t%li\t\n",level_msg[level],msg,name_file,name_func,line_number);
            done++;
        }
        //Else, do nothing
    }
    return done;
}


#else
//Size that should be used to format the time if using date and time type
static uint8_t size_time = 0;

/** ------------------------------------------------------------------------------------------- *
 *  date_and_time                                                                               *
 *                                                                                              *
 *  Retrieve and format the current date and time into a string                                 *
 *                                                                                              *
 *  @param on_terminal  boolean to know whether the time will be printed on terminal            *
 *                      0 = not on terminal                                                     *
 *                                                                                              *
 *  @return NULL        error                                                                   *
 *  @return formated    whole time's data parsed into a string ready for the log                *
 *  ------------------------------------------------------------------------------------------- **/

char* date_and_time(const uint16_t on_terminal)
{
    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    const uint8_t amount_data = 6;
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
    if (converted == NULL)
    {
        return NULL;
    }
    if (!(size_time))
    {
        size_time = 2 + 2 + length(converted[2]) + 2 + 2 + 2 + 1 * 6;
    }
    char* formated = malloc(sizeof(char) * (size_time + 1));
    if (formated == NULL)
    {
        for (uint8_t k = 0 ; k < amount_data ; k++)
        {
            free(converted[k]);
        }
        return NULL;
    }
    for (uint8_t index_converted = 0 ; index_converted < amount_data ; index_converted++)
    {
        if (converted[index_converted] == NULL)
        {
            for (int k = 0 ; k < amount_data ; k++)
            {
                free(converted[k]);
            }
            free(formated);
            return NULL;
        }
        else if (converted[index_converted][1] == '\0')
        {
            formated[index] = '0';
            formated[index + 1] = converted[index_converted][0];
            index += 2;
        }
        else
        {
            for (uint32_t index_string = 0 ; converted[index_converted][index_string] != '\0' ; index_string++)
            {
                formated[index] = converted[index_converted][index_string];
                index++;
            }
        }
        switch(index_converted)
        {
            case 0:
            case 1:
                formated[index] = '/';
                break;
            
            case 3:
            case 4:
                formated[index] = ':';
                break;

            default:
                if (on_terminal)
                {
                    formated[index] = ' ';
                }
                else
                {
                    formated[index] = '\t';
                }
        }
        free(converted[index_converted]);
        index++;
    }
    formated[size_time - 1] = '\0';
    return formated;
}


/** ------------------------------------------------------------------------------------------- *
 *  write_date_time                                                                             *
 *                                                                                              *
 *  Write the logs when the whole date data is needed                                           *
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
 *  @retval 0       error                                                                       *
 *  @retval done    wrote the given message inside of the log                                   *
 *  ------------------------------------------------------------------------------------------- **/

int8_t write_date_time(FILE* restrict const output,const uint8_t level,const char* restrict const level_color,const char* restrict const level_msg,const char* restrict const name_file,const char* restrict const name_func,const uint64_t line_number,const char* restrict const msg)
{
    uint8_t done = 0;

    if (level != LEVEL_FOL)
    {
        if (output == stdout)
        {
            char* restrict const current_time = date_and_time(1);
            fprintf(stdout,"%s | %s%s\e[0m | %s [%s - %s:%li]\n",current_time,level_color[level],level_msg[level],msg,name_file,name_func,line_number);
            free(current_time);
            done_printing++;
        }
        else if (level_color == NULL)
        {
            char* restrict const current_time = date_and_time(0);
            fprintf(LOGFILE,"%s\t%s\t%s\t%s\t%s\t%li\n",current_time,level_msg[level],msg,name_file,name_func,line_number);
            free(current_time);
            done++;
        }
        //Else, do nothing
    }
    else
    {
        if (output == stdout)
        {
            char replace_time[size_time];
            for (int k = 0 ; k < (size_time - 1) ; k++)
            {
                replace_time[k] = ' ';
            }
            replace_time[size_time - 1] = '\0';

            fprintf(output,"%s | %s%s\e[0m | %s [%s - %s:%li]\n",replace_time,level_color[level],level_msg[level],msg,name_file,name_func,line_number);
            done++;
        }
        else if (level_color == NULL)
        {
            fprintf(LOGFILE,"\t%s\t%s\t%s\t%s\t%s\t%li\n",level_msg[level],msg,name_file,name_func,line_number);
            done++;
        }
        //Else, do nothing
    }
    return done;
}

#endif


/** ------------------------------------------------------------------------------------------- *
 *  write_connect                                                                               *
 *                                                                                              *
 *  Write a connection/disconnection message as a log                                           *
 *                                                                                              *
 *  @param msg_term     Message to print in the terminal as a log                               *
 *  @param msg_file     Message to print in the log file as a log                               *
 *                                                                                              *
 *  @retval 0           error                                                                   *
 *  @retval got_time    message printed inside as a log                                         *
 *  ------------------------------------------------------------------------------------------- **/

int8_t write_connect(const char* restrict const msg_term,const char* restrict const msg_file)
{
    int8_t got_time = 0;
    #ifndef TYPE_TIME
        float80_t current_lasts = time_lasts();
        if (current_lasts != (float80_t)0)
        {
            got_time++;
            //If initialisation of the log
            if (msg_term[6] != 'i')
            {
                #ifndef NO_TERMINAL
                    fprintf(stdout,"\n---------------------------------------\n\t%s\n---------------------------------------\n",msg_term);
                #endif

                if (LOGFILE != NULL)
                {
                    fprintf(LOGFILE,"\n---------------------------------------\n\t%s\n---------------------------------------\n",msg_file);
                }
            }
            //If leaving the log
            else{
                #ifndef NO_TERMINAL
                    fprintf(stdout,"\n---------------------------------------\n%015.9Lf\t%s\n---------------------------------------\n",current_lasts,msg_term);
                #endif

                if (LOGFILE != NULL)
                {
                    fprintf(LOGFILE,"\n---------------------------------------\n%015.9Lf\t%s\n---------------------------------------\n",current_lasts,msg_file);
                }
            }
        }
        else
        {
            #ifndef NO_TERMINAL
                fprintf(stdout,"\n---------------------------------------\n\t%s without time\n---------------------------------------\n",msg_term);
            #endif

            if (LOGFILE != NULL)
            {
                fprintf(LOGFILE,"\n---------------------------------------\n\t%s without time\n---------------------------------------\n",msg_file);
            }
        }

    #else
        const char* restrict const current_time = date_and_time(0);
        if (current_time != NULL)
        {
            got_time = 1;
        }
        
        if (got_time)
        {
            #ifndef NO_TERMINAL
                fprintf(stdout,"\n---------------------------------------\n%s\t%s\n---------------------------------------\n",current_time,msg_term);
            #endif

            if (LOGFILE != NULL)
            {
                fprintf(LOGFILE,"\n---------------------------------------\n%s\t%s\n---------------------------------------\n",current_time,msg_file);
            }
        }
        else
        {
            #ifndef NO_TERMINAL
                fprintf(stdout,"\n---------------------------------------\n\t%s without time\n---------------------------------------\n",msg_term);
            #endif

            if (LOGFILE != NULL)
            {
                fprintf(LOGFILE,"\n---------------------------------------\n\t%s without time\n---------------------------------------\n",msg_file);
            }
        }
    #endif

    return got_time;
}



/** ------------------------------------------------------------------------------------------- *
 *  connect_log                                                                                 *
 *                                                                                              *
 *  Function used to start a connection with a log file                                         *
 *                                                                                              *
 *  @param name_file    Name chosen for the log file                                            *
 *                                                                                              *
 *  @retval 0   error                                                                           *
 *  @retval 1   the file was opened                                                             *
 *  ------------------------------------------------------------------------------------------- **/

int8_t connect_log(const char* restrict const name_file)
{
    if (is_init == (int8_t)0)
    {
        if (name_file != NULL && LOGFILE == NULL)
        {
            if ((LOGFILE = fopen(name_file,"a+")) == NULL){
                return (int8_t)0;
            }
        }
        is_init = (int8_t)1;

        uint8_t got_time = (uint8_t)(clock_gettime(CLOCK_MONOTONIC,&start_time) != -1);
        if (got_time == 1)
        {
            got_time = write_connect("Logs initialized","Connection to the logfile");
            //Add something to print in case no message wase printed??
        }

        return (got_time && is_init);
    }
    return is_init;
}


/** ------------------------------------------------------------------------------------------- *
 *  disconnect_log                                                                              *
 *                                                                                              *
 *  Log a disconnection message and disconnects from the log file                               *
 *                                                                                              *
 *  @retval 0   error                                                                           *
 *  @retval 1   Disconnected from the log file                                                  *
 *  ------------------------------------------------------------------------------------------- **/

uint8_t disconnect_log(void)
{
    if (is_init != (int8_t)0)
    {
        uint8_t got_time = write_connect("Logs disabled","Disconnection from the log file");
        //Add something to print in case no message wase printed??
        if (!(got_time))
        {
            LOG_ERROR("Logs off");
        }

        if (LOGFILE != NULL)
        {
            if (close_file(LOGFILE) != 0)
            {
                LOG_ERROR("Failed to close the log file");
                return (uint8_t)0;
            }
            LOGFILE = NULL;
        }
        is_init = (int8_t)0;
        return (uint8_t)1;
    }
    return (uint8_t)0;
}


/** ------------------------------------------------------------------------------------------- *
 *  write_log                                                                                   *
 *                                                                                              *
 *  Write a message as a log                                                                    *
 *                                                                                              *
 *  @param level        Level of log                                                            *
 *  @param name_file    Name of the file which the log comes from                               *
 *  @param name_func    Name of the function which needs a log                                  *
 *  @param line_number  Line where the log function was called                                  *
 *  @param msg          Message to print in the log                                             *
 *                                                                                              *
 *  @retval 0               error                                                               *
 *  @retval done_printing   message printed inside as a log                                     *
 *  ------------------------------------------------------------------------------------------- **/

int8_t write_log(const uint8_t level,const char* restrict const name_file,const char* restrict const name_func,const uint64_t line_number,const char* restrict const msg)
{
    if (is_init != (int8_t)0)
    {
       #ifdef DEBUG_MOD
       if (level > 2)
       {
	 return (int8_t)0;
       }
        #endif

        const char* const restrict LEVEL_MSG[] = {
            "ERROR ",
            "FATAL ",
            "FOLLOW",
            "WARN  ",
            "TRACE ",
            "DEBUG ",
            "INFO  "
        };
        int8_t done_printing = 0;

        #ifndef NO_TERMINAL
            const char* const restrict LEVEL_COLOR[] = {
                "\e[0;31m",
                "\e[0;33m",
                "\e[0;34m",
                "\e[0;32m",
                "\e[0;35m",
                "\e[0;36m",
                "\e[0;37m"
            };
            #ifndef TYPE_TIME
                done_printing += write_time_lasts(stdout,level,LEVEL_COLOR,LEVEL_MSG,name_file,name_func,line_number,msg);
                
            #else
                done_printing += write_date_time(stdout,level,LEVEL_COLOR,LEVEL_MSG,name_file,name_func,line_number,msg);
                
            #endif
        #endif

        if (LOGFILE != NULL)
        {
            #ifndef TYPE_TIME
                done_printing += write_time_lasts(LOGFILE,level,NULL,LEVEL_MSG,name_file,name_func,line_number,msg);
                
            #else
                done_printing += write_date_time(LOGFILE,level,NULL,LEVEL_MSG,name_file,name_func,line_number,msg);
                
            #endif
        }
        return done_printing;
    }
    return (int8_t)0;
}
