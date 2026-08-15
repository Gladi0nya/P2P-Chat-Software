#ifndef  LOGGER_H
#define LOGGER_H

#include <stdint.h>

#define ERROR   0
#define WARNING 1
#define INFO    2
#define DEBUG   3

uint8_t startLogger(void);
uint8_t stopLogger(void);
uint8_t logMsg(const uint8_t code, const char* modname, const char* msg);

#endif
