//Protect it from being included twice
#ifndef FileActsFunctions
#define FileActsFunctions

#include <stdio.h>
#include <stdint.h>

//File closing method
#define close_file fclose




/*
    ---------------------------------------------------------------------
                        Connection preparations
    ---------------------------------------------------------------------
*/

//Checking if the character given is from the ASCII characters
uint8_t ascii_val(const char val);

//Checking whether the name provided is correct for a file or not
int8_t check_name_file(const char* const name);

/*
    ---------------------------------------------------------------------
                            Open/Close file
    ---------------------------------------------------------------------
*/
//Openning the file corresponding to the name provided
FILE* open_file(FILE* restrict log,const char* restrict const name_file,const char* restrict const type_open,const char* restrict const secured_name);

/*
    ---------------------------------------------------------------------
                        Actions on the file
    ---------------------------------------------------------------------
*/
//Get the size of the opened file
int64_t size_file(FILE* restrict log,FILE* restrict file);

//Reading the whole file content
char* content_file(FILE* restrict log,FILE* restrict file,const uint64_t len);


/*
    ---------------------------------------------------------------------
                        Actions modifying the file
    ---------------------------------------------------------------------
*/







//End of the if from the beginning of the file
#endif