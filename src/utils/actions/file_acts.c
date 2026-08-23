#include "file_acts.h"

#include <stdio.h>
#include <stdlib.h>

#define Pointer
#define String
#define LogFile
#include "../usefull_functions.h"



/** ----------------------------------------------------------------------- *
 *  ascii_val                                                                *
 *                                                                          *
 *  Checking if the character given is from the ASCII characters            *
 *                                                                          *
 *  @param val  The character to check whether it is ASCII or not           *
 *                                                                          *
 *  @retval 0 error                                                         *
 *  @retval 1 success                                                       *
 * ------------------------------------------------------------------------ **/

uint8_t ascii_val(const char val)
{
    return ('a' <= val && val <= 'z') || ('A' <= val && val <= 'Z') || ('0' <= val && val <= '9');
}


/** ----------------------------------------------------------------------- *
 *  check_name_file                                                         *
 *                                                                          *
 *  Checking whether the name provided is correct for a file or not         *
 *                                                                          *
 *  @param name The name used to open a file                                *
 *                                                                          *
 *  @retval -1  argument error                                              *
 *  @retval 0   error                                                       *
 *  @retval 1   if the char* provided is correct                            *
 * ------------------------------------------------------------------------ **/

int8_t check_name_file(const char* const name)
{
    if (name == NULL)
    {
        return (int8_t)-1;
    }
    uint8_t count_points = 0;
    for (uint16_t index = 0 ; name[index] != '\0' ; index++)
    {
        if (name[index] == '.')
        {
            count_points++;
            if (count_points > 1)
            {
                return (int8_t)0;
            }
        }
        else if (!(ascii_val(name[index])))
        {
            return (int8_t)0;
        }
    }
    return (int8_t)1;
}


/** ----------------------------------------------------------------------- *
 *  open_file                                                               *
 *                                                                          *
 *  Openning the file corresponding to the name provided                    *
 *                                                                          *
 *  @param log          File used to make logs                              *
 *  @param name_file    The name used to open a file                        *
 *  @param type_open    Method used to open the file                        *
 *                      r, w, w+, a, a+,....                                *
 *  @param secured_name Name in case the first name provided was not usable *
 *                                                                          *
 *  @return NULL        Error                                               *
 *  @return file        The opened file                                     *
 * ------------------------------------------------------------------------ **/

FILE* open_file(FILE* restrict log,const char* restrict const name_file,const char* restrict const type_open,const char* restrict const secured_name)
{
    if (name_file != NULL && type_open != NULL){
        if (!(check_name_file(name_file)))
        {
            if (secured_name == NULL)
            {
                edit_log(log,"open_file",0,"Name given is not valid");
            }
            else
            {
                if (!(check_name_file(secured_name)))
                {
                    edit_log(log,"open_file",0,"The security name given is not valid");
                }
                else
                {
                    FILE* restrict file = fopen(name_file,type_open);
                    if (file != NULL){
                        return file;
                    }
                    edit_log(log,"open_file",0,"Failed to open the given file");
                    close_file(file);
                }
            }
        }
        else
        {
            FILE* restrict file = fopen(name_file,type_open);
            if (file != NULL){
                return file;
            }
            edit_log(log,"open_file",0,"Failed to open the given file");
            close_file(file);
        }
    }
    return NULL;
}


/** ----------------------------------------------------------------------- *
 *  size_file                                                               *
 *                                                                          *
 *  Get the size of the opened file                                         *
 *                                                                          *
 *  @param log          File used to make logs                              *
 *  @param file         File which trying to know the size of               *
 *                                                                          *
 *  @retval -1          Error                                               *
 *  @retval length      The size of the FILE* provided                      *
 * ------------------------------------------------------------------------ **/

int64_t size_file(FILE* restrict log,FILE* restrict file)
{
    int64_t length = -1;
    if (file != NULL)
    {
        //Going at the end of the file (moved by 0)
        fseek(file,0,SEEK_END);
        //Getting the amount of characters the given file has
        length = ftell(file);
        //Going at the beginning of the file (moved by 0)
        fseek(file,0,SEEK_SET);
    }
    else
    {
        edit_log(log,"size_file",0,"File provided does not exists");
    }
    return length;
}


/** ----------------------------------------------------------------------- *
 *  content_file                                                            *
 *                                                                          *
 *  Reading the whole file content                                          *
 *                                                                          *
 *  @param log          File used to make logs                              *
 *  @param file         File which trying to get the content of             *
 *  @param len          Size of the file given                              *
 *                                                                          *
 *  @return -1          Error                                               *
 *  @return length      The size of the FILE* provided                      *
 * ------------------------------------------------------------------------ **/

char* content_file(FILE* restrict log,FILE* restrict file,const uint64_t len)
{
    if (file != NULL && len > 0)
    {
        char* restrict const content = malloc(sizeof(char) * (len + 1));
        if (content == NULL)
        {
            free(content);
            edit_log(log,"content_file",0,"Memory allocation failed");
            return NULL;
        }
        content[len] = '\0';
        //Reading the content of the file and placing (reading characters of 1 byte)
        fread(content,1,len,file);
        return content;
    }
    edit_log(log,"content_file",0,"At least one of the provided arguments is not right");
    return NULL;
}
