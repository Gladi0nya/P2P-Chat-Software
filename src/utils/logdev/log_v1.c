#include "log.h"

#include <stdlib.h>
#include <time.h>

#define String
#define Pointer
#define File
#include "../usefull_functions.h"




//Debug mode is declared as turned off
uint8_t debug_mode = 0;
//Size that should be used to format the time
uint8_t size_time = 0;




/** ------------------------------------------------------------------------------------------- *
 *  type_edit                                                                                   *
 *                                                                                              *
 *  Choosing the kind of edit which edit_log() has been called for                              *
 *                                                                                              *
 *  @param type      type of log which has to be done                                           *
 *                                                                                              *
 *  @return type_msg    the converted choice                                                    *
 *  ------------------------------------------------------------------------------------------- **/

char* type_edit(const int8_t type)
{
    char* type_msg;
    switch(type)
    {
        case 0:
            type_msg = "ERROR";
            break;

        case 1:
            type_msg = "WARN";
            break;

        case 2:
            type_msg = "TRACE";
            break;
        
        case 3:
            type_msg = "DEBUG";
            break;

        case 4:
            type_msg = "FATAL";
            break;

        case 5:
            type_msg = "FOLLOW";
            break;

        case -5:
            type_msg = "CONNECTION";
            break;
            
        case -6:
            type_msg = "DISCONNECTION";
            break;

        
        case -1:
            type_msg = "INFO";
            break;

        default:
            type_msg = "INFO";
    };
    return type_msg;
}


/** ------------------------------------------------------------------------------------------- *
 *  get_time                                                                                    *
 *                                                                                              *
 *  Getting and formating the current time into a string                                        *
 *                                                                                              *
 *  @return formated    whole time's data parsed into a string ready for the log                *
 *  ------------------------------------------------------------------------------------------- **/

char* get_time()
{
    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    const uint8_t amount_data = 6;
    char* converted[7] = {int_to_char((int32_t)time->tm_mday),int_to_char((int32_t)time->tm_mon + 1),int_to_char((int32_t)(time->tm_year + 1900)),int_to_char((int32_t)time->tm_hour),int_to_char((int32_t)time->tm_min),int_to_char((int32_t)time->tm_sec),NULL};
    uint8_t index = 0;
    if (converted == NULL)
    {
        return NULL;
    }
    if (!(size_time))
    {
        size_time = 2 + 2 + length(converted[2]) + 2 + 2 + 2 + 1 * 5;
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
                formated[index] = '\t';
        }
        free(converted[index_converted]);
        index++;
    }
    formated[size_time - 1] = '\0';
    return formated;
}




/** ------------------------------------------------------------------------------------------- *
 *  connect_log                                                                                 *
 *                                                                                              *
 *  Function used to start a connection with a log file                                         *
 *                                                                                              *
 *  @param name_file    Name chosen for the log file                                            *
 *  @param type_open    String indicating the mod used to open the log file                     *
 *                      a, a+, w, w+, etc   -   avoid r, r+ and other mods without edition      *
 *  @param debug_mod    Boolean used to choose whether debug logs will be written or not        *
 *                                                                                              *
 *  @return NULL        error                                                                   *
 *  @return opened      The opened FILE*                                                        *
 *  ------------------------------------------------------------------------------------------- **/

FILE* connect_log(const char* const name_file,const char* const type_open,const uint8_t debug_mod)
{
    FILE* restrict opened;
    opened = open_file(NULL,name_file,type_open,"logFile.log");
    edit_log(opened,NULL,(const int8_t)-5,name_file);
    debug_mode = debug_mod;
    return opened;
}


/** ------------------------------------------------------------------------------------------- *
 *  disconnect_log                                                                              *
 *                                                                                              *
 *  Log a disconnection message and disconnects from the log file                               *
 *                                                                                              *
 *  @param used_file    FILE* linked to the log file                                            *
 *                                                                                              *
 *  @retval 0   error                                                                           *
 *  @retval 1   Disconnected from the log file                                                  *
 *  ------------------------------------------------------------------------------------------- **/

uint8_t disconnect_log(FILE* restrict used_file)
{
    if (used_file != NULL)
    {
        edit_log(used_file,NULL,(const int8_t)-6,NULL);
        close_file(used_file);
        used_file = NULL;
        return (uint8_t)1;
    }
    return (uint8_t)0;
}


/** ------------------------------------------------------------------------------------------- *
 *  edit_log                                                                                    *
 *                                                                                              *
 *  Edit the provided log file                                                                  *
 *                                                                                              *
 *  @param to_edit    FILE* linked to the log file                                              *
 *  @param name_func  Name of the function which called edit_log                                *
 *  @param type       Type of edition of the log file                                           *
 *                    0 : "ERROR"   1 : "WARN"   2 : "TRACE"   3 : "DEBUG"   4 : "FATAL"        *
 *                    5 : "FOLLOW"   -5 : "CONNECTION"   -6 : "DISCONNECTION"   -1 : "INFO"     *
 *  @param msg        Message given to be printed as an information following the log           *
 *                                                                                              *
 *  @retval -1  error with the arguments                                                        *
 *  @retval 0   error                                                                           *
 *  @retval 1   message printed inside of the log file                                          *
 *  ------------------------------------------------------------------------------------------- **/

int8_t edit_log(FILE* restrict to_edit,const char* const name_func,const int8_t type,const char* const msg)
{
    if (to_edit != NULL)
    {
        char* time = get_time();
        char* type_msg = type_edit(type);
        if (type == -5)
        {
            fprintf(to_edit,"-----------------------------------------------------------------------------------------------\n%s\tConnection to \"%s\" file\n---------------------------------------\n",time,msg);
        }
        else if (type == -6)
        {
            fprintf(to_edit,"---------------------------------------\n%s\tDisconnection from the log file\n-----------------------------------------------------------------------------------------------\n",time);
        }
        else if (type != 5 && debug_mode)
        {
            if (name_func != NULL)
            {
                if (msg != NULL)
                {
                    fprintf(to_edit,"%s\t%s\t%s\tMsg : %s\n",time,type_msg,name_func,msg);
                }
                else
                {
                    fprintf(to_edit,"%s\t%s\t%s\n",time,type_msg,name_func);
                }
            }
            else
            {
                if (msg != NULL)
                {
                    fprintf(to_edit,"%s\t\t%s\tMsg : %s\n",time,type_msg,msg);
                }
                else
                {
                    fprintf(to_edit,"%s\t\t%s\n",time,type_msg);
                }
            }
        }
        else if (type == 5)
        {
            if (msg != NULL)
            {
                fprintf(to_edit,"\t\t\t\t%s\n",msg);
            }
            else
            {
                free(time);
                return (int8_t)0;
            }
        }
        else
        {
            free(time);
            return (int8_t)0;
        }
        //Push what fprintf wrote in the log file
        fflush(to_edit);
        free(time);
        return (int8_t)1;
    }
    return (int8_t)-1;
}
