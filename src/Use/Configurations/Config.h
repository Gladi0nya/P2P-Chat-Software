//Protect it from being included twice
#pragma once

/*
    ---------------------------------------------------------------------
                    Including libraries of myself
    ---------------------------------------------------------------------
*/

//To have some string's actions
#define String
//For memory allocation
#define Pointer
//To use file actions
#define File
//To edit the log file used inside of the main
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
                            Data structures
    ---------------------------------------------------------------------
*/

//Definition for a new type which will contain data from external file
struct Config{
    //Name of the database in use
    char* database;
    //User used to open the database
    char* user;
    //Password associated to that user
    char* password;
    //Defining the host in use
    char* host;
};
typedef struct Config Config;



/*
    ---------------------------------------------------------------------
                        Functions over that structure
    ---------------------------------------------------------------------
*/

//Initialisation of a Congif*
Config* initConfig(FILE* log);
//Free the given Config*
void freeConfig(Config** toFree);

//Read the configuration file's data
int getConfig(FILE* log,char* fichier,Config* cfg);