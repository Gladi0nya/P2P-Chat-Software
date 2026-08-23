#include "config.h"

#include <stdio.h>
#include <stdlib.h>


#define String
#define Pointer
#define File
#define LogFile
#include "../usefull_functions.h"




/** ------------------------------------------------------------------------------------------- *
 *  init_config                                                                                 *
 *                                                                                              *
 *  Initialisation of a Congif*                                                                 *
 *  all of the data is set to NULL                                                              *
 *                                                                                              *
 *  @param log          File used to make logs                                                  *
 *                                                                                              *
 *  @return NULL        Error                                                                   *
 *  @return init        The made Config*                                                        *
 *  ------------------------------------------------------------------------------------------- **/

Config* init_config(FILE* restrict log)
{
    Config* restrict const init = malloc(sizeof(Config));
    if (init != NULL)
    {
        init->database = NULL;
        init->user = NULL;
        init->password = NULL;
        init->host = NULL;
        return init;
    }
    edit_log(log,"init_config",(const int8_t)0,"Memory allocation failed for Config*");
    return NULL;
}


/** ------------------------------------------------------------------------------------------- *
 *  free_config                                                                                 *
 *                                                                                              *
 *  Free and set to NULL the Congif* given                                                      *
 *                                                                                              *
 *  @param to_free         The Config* to be freed                                              *
 *  ------------------------------------------------------------------------------------------- **/

void free_config(Config* restrict const to_free)
{
    if (to_free->database != NULL)
    {
        free(to_free->database);
    }
    if (to_free->host != NULL)
    {
        free(to_free->host);
    }
    if (to_free->user != NULL)
    {
        free(to_free->user);
    }
    if (to_free->password != NULL)
    {
        free(to_free->password);
    }
    free(to_free);
}


/** ------------------------------------------------------------------------------------------- *
 *  get_config                                                                                  *
 *                                                                                              *
 *  Read the configuration file's data and gives the Config* provided the data from the file    *
 *                                                                                              *
 *  @param log      File used to make logs                                                      *
 *  @param to_free  The Config* to be freed                                                     *
 *  @param cfg      The Config* which will be edited with the given file's value                *
 *                                                                                              *
 *  @retval -1  error in arguments                                                              *
 *  @retval 0   error                                                                           *
 *  @retval 1   done                                                                            *
 *  ------------------------------------------------------------------------------------------- **/

int8_t get_config(FILE* restrict log,const char* const restrict name_file,Config* restrict const cfg)
{
    if (cfg != NULL && name_file != NULL)
    {
        FILE* restrict config_file = open_file(log,name_file,"r",NULL);
        if (config_file == NULL)
        {
            edit_log(log,"get_config",(const int8_t)0,"Failed to open the given Config file");
            return 0;
        }
        const int64_t size_config_file = size_file(log,config_file);
        char* restrict const content_config = content_file(log,config_file,size_config_file);
        close_file(config_file);
        if (content_config == NULL)
        {
            edit_log(log,"get_config",(const int8_t)0,"Failed to get the content of the file");
            return 0;
        }

        char* restrict temp_string = NULL;
        char* restrict temporary;
        int64_t temp_index;
        int8_t compared;
        char searched[] = {'=','\n','\0'};
        const char* restrict const key_words[4] = {"database","host","user","password"};
        const int size_key_words = 4;

        for (int64_t index_config_file = 0 ; index_config_file < size_config_file ; )
        {
            if (content_config[index_config_file] != '#' && content_config[index_config_file] != '[' && content_config[index_config_file] != '\n' && content_config[index_config_file] != '=')
            {
                temp_index = find_one_among_list(content_config,searched,index_config_file);
                if (temp_index >= 0)
                {
                    if (temp_string == NULL)
                    {
                        temp_string = take_part_len(content_config,index_config_file,temp_index - 1,size_config_file);
                        index_config_file = temp_index;
                    }
                    else
                    {
                        for (uint8_t index_key = 0 ; index_key < size_key_words ; index_key++)
                        {
                            compared = comparison(temp_string,key_words[index_key]);
                            if (compared == 1)
                            {
                                temporary = take_part_len(content_config,index_config_file,temp_index - 1,size_config_file);
                                if (index_key == (uint8_t)0)
                                {
                                    cfg->database = temporary;
                                }
                                else if (index_key == (uint8_t)1)
                                {
                                    cfg->host = temporary;
                                }
                                else if (index_key == (uint8_t)2)
                                {
                                    cfg->user = temporary;
                                }
                                else if (index_key == (uint8_t)3)
                                {
                                    cfg->password = temporary;
                                }
                                index_config_file = temp_index;
                                break;
                            }
                            else if (compared == -1)
                            {
                                break;
                            }
                        }
                        free(temp_string);
                        temp_string = NULL;
                    }
                }
                else if (temp_index == -2)
                {
                    edit_log(log,"get_config",(const int8_t)0,"Arguments provided to find_one_among_list() were incorrect");
                }
                else
                {
                    edit_log(log,"get_config",(const int8_t)0,"None of the searched characters were found");
                }
            }
            else if (content_config[index_config_file] == '#' || content_config[index_config_file] == '[')
            {
                edit_log(log,"get_config",(const int8_t)3,"Current line has no use");
                index_config_file = find_char(content_config,'\n',index_config_file);
                if (index_config_file == -1)
                {
                    edit_log(log,"get_config",(const int8_t)0,"End of the file, no more lines are available");
                    break;
                }
                if (index_config_file == -2)
                {
                    edit_log(log,"get_config",(const int8_t)0,"At least one of the arguments provided is NULL (to find_char())");
                    break;
                }
                index_config_file++;
            }
            else
            {
                edit_log(log,"get_config",(const int8_t)3,"Current character has no use");
                index_config_file++;
            }
        }
        free(temp_string);
        free(content_config);

        if (cfg->database != NULL && cfg->host != NULL && cfg->user != NULL && cfg->password != NULL)
        {
            return (const int8_t)1;
        }
        free(cfg->database);
        free(cfg->host);
        free(cfg->user);
        free(cfg->password);
        return (const int8_t)0;
    }
    return (const int8_t)-1;
}