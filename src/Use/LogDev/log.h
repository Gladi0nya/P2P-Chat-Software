//Protect it from being included a few times
#pragma once

/*
    ---------------------------------------------------------------------
                    Including a C standard libraries
    ---------------------------------------------------------------------
*/
//time is to have time landmard
#include <time.h>
/*
    ---------------------------------------------------------------------
                    Including libraries of myself
    ---------------------------------------------------------------------
*/
//StringActs is to have some string's actions
#define String
//For memory allocation
#define Pointer
//To use file actions
#define File

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
                        Preparation to edit
    ---------------------------------------------------------------------
*/

//Choosing the kind of edit which editLog() has been called for
char* typeEdit(int type);

//Getting and formating the current time into a string
char* getTime();


/*
    ---------------------------------------------------------------------
                        Libraries' actions
    ---------------------------------------------------------------------
*/

//Start a connection with the log file 
FILE* connectLog(char** nameFile,int len,char* typeOpen,int debugMod);
//Disconnection from the log file
int disconnectLog(FILE* usedFile);

//Edit that log file, with the function's name, the type of message and the small message that can be added
//0 : "ERROR"
//1 : "WARN"
//2 : "TRACE"
//3 : "DEBUG"
//4 : "FATAL"
//5 : "FOLLOW"
//-5 : "CONNECTION"
//-6 : "DISCONNECTION"
//-1 : "INFO"
//default : "INFO"
int editLog(FILE* toEdit,char* nameFunc,int type,char* msg);