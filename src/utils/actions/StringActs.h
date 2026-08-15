//Protect it from being included twice
#pragma once

/*
    ---------------------------------------------------------------------
                    Including a C standard libraries
    ---------------------------------------------------------------------
*/
//Memory actions (malloc/calloc/free and else)
#include <stdlib.h>
/*
    ---------------------------------------------------------------------
                    Including libraries of myself
    ---------------------------------------------------------------------
*/
//Actions on pointers
#define Pointer

//Including the chosen files
#include "../UsefullFunctions.h"



/*
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
----------------------- Separation pre-processor/functions ---------------------------------
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
*/





/*
    ---------------------------------------------------------------------
                                Informations
    ---------------------------------------------------------------------
*/
//Getting the length of the given char*
int length(char* charact);

//Comparing two char*s (returns 1 if they are the same)
int comparison(char* one,char* two);

//Getting the first index where the character provided is
long int findChar(char* string,char searchingFor,long int start);

//Getting the first index where one of the characters provided is
long int findOneAmongList(char* string,char searchingFor[],long int start);

/*
//Getting the first index where the string provided is
int findString(char* string,char* searchingFor,long int start);
*/

/*
    ---------------------------------------------------------------------
                                Conversion
    ---------------------------------------------------------------------
*/
//Converting an integer into a character
char* intToChar(int toConvert);


/*
    ---------------------------------------------------------------------
                        Creating & Initialising char*
    ---------------------------------------------------------------------
*/
//Creates and initialise with the given value a new string
char* createInitString(char* fillIn);

//Creates a string out of a given part of another one looping over the whole string (except higher index than the lastCopy's one)
char* takePart(char* bigString,long int firstCopy,long int lastCopy);

//Creates a string out of a given part of another one only looping over the part to copy
char* takePartLen(char* bigString,long int firstCopy,long int lastCopy,long int lenBigString);



/*
    ---------------------------------------------------------------------
                            String modifications
    ---------------------------------------------------------------------
*/
//Modifying the char*
char* modifyChar(char* instruction,char** toPlace,char toReplace);

//Place down the second provided char* at the (size-1) index inside a char*
int placeInString(char** string,char* toPlace,int currentSize);
