#ifndef UserInputsFunctions
#define UserInputsFunctions

#include <stdint.h>
#include <stdio.h>



/** ------------------------------------------------------------------------------------------- *
 *  clear_stdin                                                                                  *
 *                                                                                              *
 *  Clearing stdin from everything the user may have entered                                    *
 *  ------------------------------------------------------------------------------------------- **/

static inline void clear_stdin()
{
    int32_t stdVal;
    while ((stdVal = getchar()) != '\n' && stdVal != EOF)
    {}
}

//Letting user enter an integer
int32_t get_user_int(const char* const quest);

//Letting user enter a character
char get_user_char(const char* const quest);

//Letting user enter a string
char* get_user_string(const char* const quest);


#endif