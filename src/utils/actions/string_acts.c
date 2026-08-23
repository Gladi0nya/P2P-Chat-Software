#include "string_acts.h"

#include <stdio.h>
#include <stdlib.h>

#define Pointer
#include "../usefull_functions.h"




/** ----------------------------------------------------------------------------------------------- *
 *  length                                                                                          *
 *                                                                                                  *
 *  Getting the size of a char*                                                                     *
 *                                                                                                  *
 *  @param string    The char* trying to have the size of                                           *
 *                                                                                                  *
 *  @retval 0       error or length or 0                                                            *
 *  @retval size    converted value                                                                 *
 *  ----------------------------------------------------------------------------------------------- **/

uint32_t length(const char* const string)
{
    uint32_t size = 0;
    if (string != NULL)
    {
        while (string[size] != '\0')
        {
            size++;
        }
    }
    return size;
}


/** ----------------------------------------------------------------------------------------------- *
 *  comparison                                                                                      *
 *                                                                                                  *
 *  Comparing two char*s (returns 1 if they are the same)                                           *
 *                                                                                                  *
 *  @param one    first string to be compared                                                       *
 *  @param two    second string to be compared                                                      *
 *                                                                                                  *
 *  @retval -1  error of arguments                                                                  *
 *  @retval 0   the two strings are differents                                                      *
 *  @retval 1   the two strings are the same                                                        *
 *  ----------------------------------------------------------------------------------------------- **/

int8_t comparison(const char* const one,const char* const two)
{
    if (one != NULL && two != NULL)
    {
        int32_t index = 0;
        while (one[index] != '\0' || two[index] != '\0')
        {
            if (one[index] != two[index])
            {
                return (int8_t)0;
            }
            index++;
        }
        if (one[index] == two[index]){
            return (int8_t)1;
        }
        return (int8_t)0;
    }
    return (int8_t)-1;
}


/** ----------------------------------------------------------------------------------------------- *
 *  find_char                                                                                        *
 *                                                                                                  *
 *  Getting the first index where the character provided is                                         *
 *                                                                                                  *
 *  @param string           string in which searching for a character                               *
 *  @param searching_for    the character being searched for                                        *
 *  @param start            starting index to search after the given char                           *
 *                                                                                                  *
 *  @retval -1              error of arguments                                                      *
 *  @retval -2              the character could not be found                                        *
 *  @retval index_string    the index where the character searched was found                        *
 *  ----------------------------------------------------------------------------------------------- **/

int64_t find_char(const char* const string,const char searching_for,const int64_t start)
{
    if (string != NULL && start >= 0)
    {
        for (int64_t index_string = start ; string[index_string] != '\0' ; index_string++)
        {
            if (string[index_string] == searching_for)
            {
                return index_string;
            }
        }
        return (int64_t)-2;
    }
    return (int64_t)-1;
}


/** ----------------------------------------------------------------------------------------------- *
 *  find_one_among_list                                                                             *
 *                                                                                                  *
 *  Getting the first index where one of the characters provided is                                 *
 *                                                                                                  *
 *  @param string           string in which searching for a character                               *
 *  @param searching_for    the list of characters being searched for                               *
 *  @param start            starting index to search after the given chars                          *
 *                                                                                                  *
 *  @retval -1              error of arguments                                                      *
 *  @retval -2              none of the characters were found                                       *
 *  @retval index_string    the index where the character searched was found                        *
 *  ----------------------------------------------------------------------------------------------- **/

int64_t find_one_among_list(const char* const string,const char* const searching_for,const int64_t start)
{
    if (string != NULL && searching_for != NULL && start >= 0)
    {
        for (int64_t index_string = start ; string[index_string] != '\0' ; index_string++)
        {
            for (int64_t index_search = 0 ; searching_for[index_search] != '\0' ; index_search++)
            {
                if (string[index_string] == searching_for[index_search])
                {
                    return index_string;
                }
            }
        }
        return (int64_t)-2;
    }
    return (int64_t)-1;
}

/*
//Getting the first index where the string provided is (Boyer-Moore algorithm)
int find_string(char* string,char* searching_for){
    //If the given arguments are usable
    if (string != NULL && searching_for != NULL){
        //Length of the string trying to find
        int len_searched_for = length(searching_for);
        //Initialising the index for searching_for
        int index_search;
        //Loop over the string
        for (int index_string = 0 ; string[index_string] != '\0' ; index_string++){
            //Loop over the characters searching to find
            for (index_search = 0 ; searching_for[index_search] != '\0' ; index_search++){
                //If one of the characters is different
                if (string[index_string + index_search] != searching_for[index_search]){
                    //Breaking out of the loop
                    break;
                }
            }
            //If the loop went until the end
            if (searching_for[index_search] == '\0'){
                //Returns the position
                return index_string;
            }
        }
        //Returns -1
        return -1;
    }
    //Returns -2
    return -2;
}
*/



/** ----------------------------------------------------------------------------------------------- *
 *  int_to_char                                                                                     *
 *                                                                                                  *
 *  Converting an integer into a character                                                          *
 *                                                                                                  *
 *  @param to_convert   integer to convert into a string                                            *
 *                                                                                                  *
 *  @return NULL    error                                                                           *
 *  @return result  the built string                                                                *
 *  ----------------------------------------------------------------------------------------------- **/

char* int_to_char(int32_t to_convert)
{
    if (to_convert == 0)
    {
        return create_init_string("0");
    }
    uint32_t nbDigits = 0;
    int32_t temp = to_convert;
    while (temp > 0)
    {
        nbDigits++;
        temp /= 10;
    }
    char* result = malloc(sizeof(char) * (nbDigits + 1));
    if (result != NULL)
    {
        for (uint32_t index = (nbDigits - 1) ; to_convert > 0 ; index--)
        {
            //printf("nbDigits = %i\tindex = %i\t(%i mod 10) + '0' : %c\tEntire : %s\n",nbDigits,index,to_convert,(to_convert % 10) + '0',result);
            result[index] = (to_convert % 10) + '0';
            to_convert /= 10;
        }
        result[nbDigits] = '\0';
    }
    //printf("result = %s\n",result);
    return result;
}


/** ----------------------------------------------------------------------------------------------- *
 *  create_init_string                                                                              *
 *                                                                                                  *
 *  Creates and initialise with the given value a new string                                        *
 *                                                                                                  *
 *  @param fill_in   String to copy inside of the made pointer                                      *
 *                                                                                                  *
 *  @return NULL        error                                                                       *
 *  @return new_string  the built string                                                            *
 *  ----------------------------------------------------------------------------------------------- **/

char* create_init_string(const char* const fill_in)
{
    if (fill_in != NULL)
    {
        uint64_t current_size = length(fill_in) + 1;
        char* new_string = malloc(sizeof(char) * current_size);
        if (new_string != NULL)
        {
            for (uint64_t index = 0 ; index < current_size ; index++)
            {
                new_string[index] = fill_in[index];
            }
            new_string[current_size - 1] = '\0';
        }
        return new_string;
    }
    return NULL;
}


/** ----------------------------------------------------------------------------------------------- *
 *  take_part                                                                                       *
 *                                                                                                  *
 *  Creates a string out of a given part of another one looping over the whole string               *
 *  (except higher index than the last_copy's one)                                                  *
 *                                                                                                  *
 *  @param big_string   string to partially copy inside of the made pointer                         *
 *  @param first_copy   index where the copy has to begin                                           *
 *  @param last_copy    index where the copy has to end                                             *
 *                                                                                                  *
 *  @return NULL            error                                                                   *
 *  @return small_string    the built string                                                        *
 *  ----------------------------------------------------------------------------------------------- **/

char* take_part(const char* const big_string,const uint64_t first_copy,const uint64_t last_copy)
{
    if (big_string != NULL && last_copy > first_copy)
    {
        uint64_t len = last_copy - first_copy + 1;
        char* small_string = malloc(sizeof(char) * (len));
        if (small_string != NULL)
        {
            small_string[len - 1] = '\0';
            for (uint64_t index = 0 ; (big_string[index] != '\0' && index < last_copy) ; index++)
            {
                if (index >= first_copy)
                {
                    small_string[index - first_copy] = big_string[index];
                }
            }
        }
        return small_string;
    }
    return NULL;
}


/** ----------------------------------------------------------------------------------------------- *
 *  take_part_len                                                                                   *
 *                                                                                                  *
 *  Creates a string out of a given part of another one only looping over the part to copy          *
 *  (except higher index than the last_copy's one)                                                  *
 *                                                                                                  *
 *  @param big_string       string to partially copy inside of the made pointer                     *
 *  @param first_copy       index where the copy has to begin                                       *
 *  @param last_copy        index where the copy has to end                                         *
 *  @param len_big_string   size of the string provided                                             *
 *                                                                                                  *
 *  @return NULL            error                                                                   *
 *  @return small_string    the built string                                                        *
 *  ----------------------------------------------------------------------------------------------- **/

char* take_part_len(const char* const big_string,const uint64_t first_copy,const uint64_t last_copy,const uint64_t len_big_string)
{
    if (big_string != NULL && first_copy > 0 && first_copy < len_big_string && last_copy >= first_copy && first_copy < len_big_string)
    {
        uint64_t len = last_copy - first_copy + 1;
        char* small_string = malloc(sizeof(char) * (len + 1));
        if (small_string != NULL)
        {
            small_string[len] = '\0';
            for (uint64_t index = first_copy ; index <= last_copy ; index++)
            {
                small_string[index - first_copy] = big_string[index];
            }
        }
        return small_string;
    }
    return NULL;
}


/** ----------------------------------------------------------------------------------------------- *
 *  modify_char                                                                                     *
 *                                                                                                  *
 *  Modifying the char* provided with the list of char* provided on the index of the char provided  *
 *                                                                                                  *
 *  @param to_change    string which will be modified                                               *
 *  @param to_place     list of strings to place inside of the main string                          *
 *  @param to_replace   character searching after to place on the new string                        *
 *                                                                                                  *
 *  @return NULL            error                                                                   *
 *  @return small_string    the built string                                                        *
 *  ----------------------------------------------------------------------------------------------- **/

char* modify_char(char* to_change,const char* const * const to_place,const char to_replace)
{
    if (to_change != NULL && to_place != NULL)
    {
        uint64_t len_to_change = (uint64_t)(length(to_change) + 1);
        char* changed_string = malloc(sizeof(char) * len_to_change);
        if (changed_string == NULL)
        {
            return NULL;
        }
        uint64_t index_changed_string = 0;
        uint64_t index_to_place = 0;
        uint64_t len_to_place;
        for (uint64_t elt_to_change = 0 ; to_change[elt_to_change] != '\0' ; elt_to_change++)
        {
            if ((to_change[elt_to_change] == to_replace) && (to_place[index_to_place] != NULL))
            {
                len_to_place = length(to_place[index_to_place]);
                len_to_change += len_to_place - 1;

                changed_string = realloc(changed_string,sizeof(char) * len_to_change);
                if (changed_string == NULL)
                {
                    return NULL;
                }
                for (uint64_t k = 0 ; k < len_to_place ; k++)
                {
                    changed_string[index_changed_string] = to_place[index_to_place][k];
                    index_changed_string++;
                }
                index_to_place++;
            }
            else
            {
                changed_string[index_changed_string] = to_change[elt_to_change];
                index_changed_string++;
            }
            changed_string[len_to_change - 1] = '\0';
        }
        changed_string[len_to_change - 1] = '\0';
        return changed_string;
    }
    return NULL;
}


/** ----------------------------------------------------------------------------------------------- *
 *  place_in_string                                                                                 *
 *                                                                                                  *
 *  Place down the second provided char* at the (size-1) index inside a char*                       *
 *                                                                                                  *
 *  @param string           string which will be modified                                           *
 *  @param to_place         string which will be placed down                                        *
 *  @param current_index    currently used index                                                    *
 *                          has to be before the index of the '\0' character's index                *
 *                                                                                                  *
 *  @retval -1              error on arguments                                                      *
 *  @retval 0               error                                                                   *
 *  @retval current_size    the new size of the string                                              *
 *  ----------------------------------------------------------------------------------------------- **/

uint64_t place_in_string(char** restrict string,const char* restrict const to_place,const uint64_t current_index)
{
    if (to_place != NULL && string != NULL)
    {
        uint64_t current_size = current_index + length(to_place) + 1;
        *string = realloc((*string),current_size);
        if (*string == NULL)
        {
            return 0;
        }
        for (uint64_t index = current_index ; index < current_size ; index++)
        {
            (*string)[index] = to_place[index - current_index];
        }
        (*string)[current_size - 1] = '\0';
        return current_size;
    }
    return 0;
}
