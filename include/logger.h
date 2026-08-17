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

#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

#define LOG_LEVEL_ERROR   0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_DEBUG   3

uint8_t log_init(void);
uint8_t log_shutdown(void);
uint8_t log_write(const uint8_t level, const char* filename, const uint64_t line, const char* msg);

#define LOG_ERROR(msg)   log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, msg)
#define LOG_WARNING(msg) log_write(LOG_LEVEL_WARNING, __FILE__, __LINE__, msg)
#define LOG_INFO(msg)    log_write(LOG_LEVEL_INFO, __FILE__, __LINE__, msg)
#define LOG_DEBUG(msg)   log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, msg)

#endif
