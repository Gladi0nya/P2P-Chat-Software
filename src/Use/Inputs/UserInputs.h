//Protect it from being included twice
#pragma once

/*
    ---------------------------------------------------------------------
                    Including a C standard libraries
    ---------------------------------------------------------------------
*/
//Used to have user inputs
#include <stdio.h>

/*
    ---------------------------------------------------------------------
                    Including libraries of myself
    ---------------------------------------------------------------------
*/
//Actions on pointers
#define Pointer
//Actions on char* (=string)
#define String

//Including the chosen files
#include "../UsefullFunctions.h"


/*
    ---------------------------------------------------------------------
                        Defining a constant
    ---------------------------------------------------------------------
*/
//Definition of a maximum size for users' input
#define maxUserStringSize 500




/*
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
----------------------- Separation pre-processor/functions ---------------------------------
--------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------
*/




/*
    ---------------------------------------------------------------------
                            stdin's actions
    ---------------------------------------------------------------------
*/
//Clearing stdin from everything the user may have entered
void clearStdin();

//Letting user enter an integer
int getUserInt();

//Letting user enter a character
char getUserChar();

//Letting user enter a string
char* getUserString();