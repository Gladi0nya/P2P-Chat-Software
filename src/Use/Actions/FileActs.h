//Protect it from being included twice
#pragma once

/*
    ---------------------------------------------------------------------
                    Including a C standard libraries
    ---------------------------------------------------------------------
*/
//Files openning/closing/writing and other actions
#include <stdio.h>

/*
    ---------------------------------------------------------------------
                    Including libraries of myself
    ---------------------------------------------------------------------
*/
//For memory allocation
#define Pointer
//StringActs is to have some string's actions
#define String
//Writing inside of the log file
#define LogFile

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
                        Connection preparations
    ---------------------------------------------------------------------
*/

//Checking if the character given is from the ASCII characters
int asciiVal(char val);

//Checking whether the name provided is correct for a file or not
int checkName(char* name);

/*
    ---------------------------------------------------------------------
                            Open/Close file
    ---------------------------------------------------------------------
*/
//Truly openning the given file (no checking of the name)
FILE* trueOpenFile(FILE* log,char* nameFile,char* typeOpen);
//Openning the file corresponding to the name provided
FILE* openFile(FILE* log,char* nameFile,char* typeOpen,char* securedName);

//Close the given file
void closeFile(FILE* toClose);


/*
    ---------------------------------------------------------------------
                        Actions on the file
    ---------------------------------------------------------------------
*/
//Get the size of the opened file
long int sizeFile(FILE* log,FILE* file);

//Reading the whole file content
char* contentFile(FILE* log,FILE* file,long int len);


/*
    ---------------------------------------------------------------------
                        Actions modifying the file
    ---------------------------------------------------------------------
*/
