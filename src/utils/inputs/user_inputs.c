#include "user_inputs.h"

#include <stdlib.h>

#define Pointer
#define String
#include "../usefull_functions.h"


//Maximum size for users' input
#define MAX_USER_STRING_SIZE 500





/** ------------------------------------------------------------------------------------------- *
 *  get_user_int                                                                                *
 *                                                                                              *
 *  Prints what was given as an argument                                                        *
 *  Letting user choose his action and getting the integer he provided                          *
 *                                                                                              *
 *  @param quest    The string which will be printed before the user gets to do anything        *
 *                                                                                              *
 *  @retval -10         error                                                                   *
 *  @retval return_val  The integer the user typed in                                           *
 *  ------------------------------------------------------------------------------------------- **/

int32_t get_user_int(const char* const quest)
{
    int32_t return_val;
    printf("%s",quest);
    if (!(scanf("%i",&return_val)))
    {
        clear_stdin();
        return (int32_t)-10;
    }
    clear_stdin();
    return return_val;
}


/** ------------------------------------------------------------------------------------------- *
 *  get_user_char                                                                               *
 *                                                                                              *
 *  Prints what was given as an argument                                                        *
 *  Letting user choose his action and getting the character he provided                        *
 *                                                                                              *
 *  @param quest    The string which will be printed before the user gets to do anything        *
 *                                                                                              *
 *  @return return_val  The characater the user typed in                                        *
 *  ------------------------------------------------------------------------------------------- **/

char get_user_char(const char* const quest)
{
    printf("%s",quest);
    const char return_val = getchar();
    clear_stdin();
    return return_val;
}


/** ------------------------------------------------------------------------------------------- *
 *  get_user_string                                                                             *
 *                                                                                              *
 *  Prints what was given as an argument                                                        *
 *  Letting user choose his action and getting the string he provided                           *
 *                                                                                              *
 *  @param quest    The string which will be printed before the user gets to do anything        *
 *                                                                                              *
 *  @return NULL        error                                                                   *
 *  @return return_val  The string the user typed in                                            *
 *  ------------------------------------------------------------------------------------------- **/

char* get_user_string(const char* const quest){
    char* return_val = malloc(sizeof(char) * MAX_USER_STRING_SIZE);
    if (return_val == NULL)
    {
        return NULL;
    }
    printf("%s",quest);

    uint32_t size = 1;
    char stdVal;
    while (((stdVal = getchar()) != '\n') && size < MAX_USER_STRING_SIZE)
    {
        size++;
        if (return_val == NULL)
        {
            clear_stdin();
            return NULL;
        }
        return_val[size - 2] = stdVal;
    }
    return_val[size - 1] = '\0';
    
    if (size < MAX_USER_STRING_SIZE)
    {
        return_val = realloc(return_val,size);
        if (return_val == NULL){
            return NULL;
        }
    }
    return return_val;
}