/**
 * @file  logger.h
 * @brief Logger definitions
 *
 * @author Tom Schmitt
 * @author Augustin Barniet
 *
 * Copyright (c) 2026 Tom Schmitt, Augustin Barniet
 * All rights reserved.
 *
 */

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
