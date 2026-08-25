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

int  rnds8(uint8_t* const restrict u8);
int rnds16(uint16_t* const restrict u16);
int rnds32(uint32_t* const restrict u32);
int rnds64(uint64_t* const restrict u64);
#endif
