//Protect it from being included twice
#ifndef ConfigFunctions
#define ConfigFunctions

#include <stdint.h>
#include <stdio.h>



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
Config* init_config(FILE* restrict log);

//Free the given Config*
void free_config(Config* restrict const to_free);

//Read the configuration file's data
int8_t get_config(FILE* restrict log,const char* const restrict name_file,Config* restrict const cfg);


#endif