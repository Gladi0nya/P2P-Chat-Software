#ifndef StringActsFunctions
#define StringActsFunctions

#include <stdint.h>


/*
    ---------------------------------------------------------------------
                                Informations
    ---------------------------------------------------------------------
*/
//Getting the length of the given char*
uint32_t length(const char* const string);

//Comparing two char*s (returns 1 if they are the same)
int8_t comparison(const char* const one,const char* const two);

//Getting the first index where the character provided is
int64_t find_char(const char* const string,const char searching_for,const int64_t start);

//Getting the first index where one of the characters provided is
int64_t find_one_among_list(const char* const string,const char* const searching_for,const int64_t start);

/*
//Getting the first index where the string provided is (Boyer-Moore algorithm)
int find_string(char* string,char* searching_for)
*/

/*
    ---------------------------------------------------------------------
                                Conversion
    ---------------------------------------------------------------------
*/
//Converting an integer into a character
char* int_to_char(int32_t to_convert);


/*
    ---------------------------------------------------------------------
                        Creating & Initialising char*
    ---------------------------------------------------------------------
*/
//Creates and initialise with the given value a new string
char* create_init_string(const char* const fill_in);

//Creates a string out of a given part of another one looping over the whole string (except higher index than the lastCopy's one)
char* take_part(const char* const big_string,const uint64_t first_copy,const uint64_t last_copy);

//Creates a string out of a given part of another one only looping over the part to copy
char* take_part_len(const char* const big_string,const uint64_t first_copy,const uint64_t last_copy,const uint64_t len_big_string);



/*
    ---------------------------------------------------------------------
                            String modifications
    ---------------------------------------------------------------------
*/
//Modifying the char*
char* modify_char(char* to_change,const char* const * const to_place,const char to_replace);

//Place down the second provided char* at the (size-1) index inside a char*
uint64_t place_in_string(char** string,const char* const to_place,const uint64_t current_index);



//End of the if from the beginning of the file
#endif