//Protect it from being included a few times
#ifndef logFilesFunctions
#define logFileFunctions

#include <stdint.h>
#include <stdio.h>


/*
    ---------------------------------------------------------------------
                        Preparation to edit
    ---------------------------------------------------------------------
*/

//Choosing the kind of edit which editLog() has been called for
char* type_edit(const int8_t type);

//Getting and formating the current time into a string
char* get_time();


/*
    ---------------------------------------------------------------------
                        Libraries' actions
    ---------------------------------------------------------------------
*/

//Start a connection with the log file 
FILE* connect_log(const char* const name_file,const char* const type_open,const uint8_t debug_mod);
//Disconnection from the log file
uint8_t disconnect_log(FILE* restrict used_file);

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
int8_t edit_log(FILE* restrict to_edit,const char* const name_func,const int8_t type,const char* const msg);



//End of the if from the beginning of the file
#endif