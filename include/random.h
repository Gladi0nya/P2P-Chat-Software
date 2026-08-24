/**
 * @file  random.h
 * @brief Random header
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

uint8_t  rnd8(void);
uint16_t rnd16(void);
uint32_t rnd32(void);
uint64_t rnd64(void);
#endif
