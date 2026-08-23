#ifndef LOG_FILES_FUNCTIONS
#define LOG_FILES_FUNCTIONS

#include <stdint.h>
#include <stdio.h>


#define LEVEL_ERR 0 //Error level
#define LEVEL_FAT 1 //Fatal level
#define LEVEL_FOL 2 //Follow level
#define LEVEL_WAR 3 //Warning level
#define LEVEL_TRA 4 //Tracing level
#define LEVEL_DEB 5 //Debug level
#define LEVEL_INF 6 //Information level

#define LOG_ERROR(msg)    write_log(LEVEL_ERR,__FILE__,__func__,__LINE__,msg)
#define LOG_FATAL(msg)    write_log(LEVEL_FAT,__FILE__,__func__,__LINE__,msg)
#define LOG_FOLLOW(msg)   write_log(LEVEL_FOL,__FILE__,__func__,__LINE__,msg)
//Unused while in debug mod
#define LOG_WARN(msg)     write_log(LEVEL_WAR,__FILE__,__func__,__LINE__,msg)
#define LOG_TRACE(msg)    write_log(LEVEL_TRA,__FILE__,__func__,__LINE__,msg)
#define LOG_DEBUG(msg)    write_log(LEVEL_DEB,__FILE__,__func__,__LINE__,msg)
#define LOG_INFO(msg)     write_log(LEVEL_INF,__FILE__,__func__,__LINE__,msg)


//Start a connection with the log file 
int8_t connect_log(const char* restrict const name_file);
//Disconnection from the log file
uint8_t disconnect_log();

//Edit that log file, with the function's name, the type of message and the small message that can be added
int8_t write_log(const uint8_t level,const char* restrict const name_file,const char* restrict const name_func,const uint64_t line_number,const char* restrict const msg);


//End of the if from the beginning of the file
#endif