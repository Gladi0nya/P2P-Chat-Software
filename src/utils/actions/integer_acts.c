#include "integer_acts.h"

#include <stddef.h>

#define String
#include "../usefull_functions.h"





/** ------------------------------------------------------------------------------------------------------- *
 *  char_to_int                                                                                             *
 *                                                                                                          *
 *  Converting a char* into a positive integer                                                              *
 *                                                                                                          *
 *  @param to_convert   The char* containing either character or integers to convert into an integer value  *
 *  @param max_length   The maximum size for the integer which will get the converted value                 *
 *                      Default value is 25 since unsigned long were used                                   *
 *                                                                                                          *
 *  @retval 0           error or value converted (if you use that function to have 0)                       *
 *  @retval created_int  converted value                                                                     *
 * -------------------------------------------------------------------------------------------------------- **/

 uint64_t char_to_int(const char* const restrict to_convert,const uint16_t max_length)
{
    uint64_t created_int = 0;
    const uint16_t max_size = 24;
    if (to_convert != NULL && max_length < max_size)
    {
        uint64_t len = length(to_convert);
        for (uint16_t index_to_convert = 0 ; index_to_convert < len ; index_to_convert++)
        {
            if (index_to_convert < max_length)
            {
                switch (to_convert[index_to_convert])
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        created_int = created_int * 10 + (to_convert[index_to_convert] - '0');
                    
                    default:
                        break;
                }
            }
            else
            {
                break;
            }
        }
    }
    return created_int;
}

/** -------------------------------------------------------------------------------------------------------- *
 *  sum_table                                                                                                *
 *                                                                                                           *
 *  Making the sum of all elements of the table given                                                        *
 *                                                                                                           *
 *  @param table_int    The character to check whether it is ASCII or not                                    *
 *  @param size_table   The maximum size for the integer which will get the converted value                  *
 *                      Default value is 25 since unsigned long were used                                    *
 *                                                                                                           *
 *  @retval 0               error or value converted (if you use that function to have 0)                    *
 *  @retval created_int     converted value                                                                  *
 * --------------------------------------------------------------------------------------------------------- **/

int32_t sum_table(const uint32_t* restrict const table_int,const uint16_t size_table)
{
    int32_t sum = 0;
    if (table_int != NULL && size_table > 0)
    {
        for (uint16_t index = 0 ; index < size_table ; index++)
        {
            sum += table_int[index];
        }
    }
    return sum;
}


/** ------------------------------------------------------------------------------------------- *
 *  inside                                                                                      *
 *                                                                                              *
 *  Check if an integer is inside a sorted list                                                 *
 *                                                                                              *
 *  @param val      Value to check whether the list is inside of the list                       *
 *  @param list     Sorted list of integers                                                     *
 *  @param size     Size of that sorted list                                                    *
 *                                                                                              *
 *  @retval -1          Error on the arguments provided                                         *
 *  @retval 0           The integer is not inside of the provided list                          *
 *  @retval current     Index where the integer was found                                       *
 *  ------------------------------------------------------------------------------------------- **/

int8_t inside(const int32_t toFind,const int32_t* restrict const list,uint32_t size)
{
    if (list != NULL && size > 0)
    {
        uint32_t beg = 0;
        uint32_t current;
        int32_t elt;
        while (beg < size)
        {
            current = (beg + size) / 2;
            elt = list[current];
            if (toFind == elt)
            {
                return current;
            }
            else if (toFind < elt)
            {
                size = current;
            }
            else
            {
                beg = current + 1;
            }
        }
        return 0;
    }
    return -1;
}