/**
 * @file  random.c
 * @brief Random number generation module
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 *
 * TODO:
 *   - x86 intrinsics detection
 *   - ARM intrinsics function + detection
 *   - RISCV intrinsics function + detection
 *   - PPC intrinsics function + detection 
 */

#include "random.h"

#ifdef __x86_64__

#include <immintrin.h>

#elif defined(__aarch64__)

#include <arm_acle.h>

#endif

static uint64_t rand64;

uint8_t rnd8(void)
{
  rand64 >>= 8;

  if (!rand64) {
    #ifdef __RDRND__
    _rdrand64_step((unsigned long long*)&rand64); 

    #elif defined(__ARM_FEATURE_RNG)

    __rndr((unsigned long long*)&rand64); // Might consider using rndrrs
    
    #endif
  }
  
  return (rand64 & 0xFF);
}

uint16_t rnd16(void)
{
  rand64 >>= 16;

  if (!(rand64 & 0xFF00)) {
    #ifdef __RDRND__
    _rdrand64_step((unsigned long long*)&rand64); 

    #elif defined(__ARM_FEATURE_RNG)

    __rndr((unsigned long long*)&rand64); // Might consider using rndrrs
    
    #endif
  }
  
  return (rand64 & 0xFFFF);
}

uint32_t rnd32(void)
{
  rand64 >>= 32;

  if (!(rand64 & 0xFF000000)) {
    #ifdef __RDRND__
    _rdrand64_step((unsigned long long*)&rand64); 

    #elif defined(__ARM_FEATURE_RNG)

    __rndr((unsigned long long*)&rand64); // Might consider using rndrrs
    
    #endif
  }
  
  return (rand64 & 0xFFFFFFFF);
}

uint64_t rnd64(void)
{

  #ifdef __RDRND__
  _rdrand64_step((unsigned long long*)&rand64); 

  #elif defined(__ARM_FEATURE_RNG)
  __rndr((unsigned long long*)&rand64); // Might consider using rndrrs
    
  #endif
  
  return rand64;
}
