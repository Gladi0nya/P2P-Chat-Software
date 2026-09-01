/**
 * @file  logger.h
 * @brief Logger definitions
 *
 * @author Augustin Barniet
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Augustin Barniet, Tom Schmitt
 * All rights reserved.
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#define _POSIX_C_SOURCE 200809L 

#include <stdint.h>

#define LOG_LEVEL_ERROR   0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_DEBUG   3

uint8_t log_init(void);
uint8_t log_shutdown(void);
uint8_t log_write(const uint8_t level, const char* restrict const filename,
				  const char* restrict const funcname, const uint64_t line,
				  const char* restrict const format,...);

#define LOG_ERROR(...)   log_write(LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) log_write(LOG_LEVEL_WARNING, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    log_write(LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...)   log_write(LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, __VA_ARGS__)

#endif
