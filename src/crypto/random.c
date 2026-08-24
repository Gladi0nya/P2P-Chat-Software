/**
 * @file  random.c
 * @brief Random number generation module
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#include "random.h"

#include <immintrin.h>

static uint64_t rand64;

uint8_t rnd8(void)
{
  rand64 >>= 8;
  
  if (!rand64) _rdrand64_step((unsigned long long*)&rand64);

  return (rand64 & 0xFF);
}

uint16_t rnd16(void)
{
  rand64 >>= 16;

  if (!(rand64 & 0xFF00)) _rdrand64_step((unsigned long long*)&rand64);

  return (rand64 & 0xFFFF);
}

uint32_t rnd32(void)
{
  rand64 >>= 32;

  if (!(rand64 & 0xFF000000)) _rdrand64_step((unsigned long long*)&rand64);

  return (rand64 & 0xFFFFFFFF);
}

uint64_t rnd64(void)
{
  _rdrand64_step((unsigned long long*)&rand64);

  return rand64;
}
