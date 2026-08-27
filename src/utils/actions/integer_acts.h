//Protect it from being included twice
#ifndef IntegerActsFunctions
#define IntegerActsFunctions

#include <stdint.h>



/*
    ---------------------------------------------------------------------
                            Conversions
    ---------------------------------------------------------------------
*/
//Converting a char* into an integer
uint64_t char_to_int(const char* const restrict to_convert,const uint16_t max_length);

/*
    ---------------------------------------------------------------------
                            Tables actions
    ---------------------------------------------------------------------
*/
//Making the sum of all elements of the table given
int32_t sum_table(const uint32_t* restrict const table_int,const uint16_t size_table);

//Check if an integer is inside a sorted list
int8_t inside(const int32_t toFind,const int32_t* restrict const list,uint32_t size);

//End of the if from the beginning of the file
#endif