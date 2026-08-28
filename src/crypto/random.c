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

#elif defined(__ANDROID__)

#include <sys/syscall.h>
#include <unistd.h>

#define getrandom(buffer, buffer_sz, flag) syscall(SYS_getrandom, buffer, buffer_sz, flag)

#elif defined(__aarch64__)

#include <arm_acle.h>

#else

#include <sys/random.h>

#endif

static uint64_t rand64 = 0ULL;

/** ----------------------------------------------------------- *
  *  rnds8                                                      *
  *                                                             *
  *  Generate crypto-secure 1 byte random number.               *
  *                                                             *
  *  @param u8 [out] Random number.                             *
  *                                                             *
  *  @retval 0 Program ran without errors.                      *
  *  @retval 1 Program ran into an error.                       *
  * ----------------------------------------------------------- **/

int rnds8(uint8_t* const restrict u8)
{
  rand64 >>= 8;

  if (!rand64) {
    #ifdef __RDRND__
    _rdrand64_step((unsigned long long*)&rand64); 

    #elif defined(__ARM_FEATURE_RNG)
    __rndr((unsigned long long*)&rand64); // Might consider using rndrrs

    #else

    if (getrandom(&rand64, sizeof(uint64_t), 0) != sizeof (uint64_t))
      return 1;
    
    #endif
  }

  *u8 = (uint8_t)(rand64 & 0xFF);
  
  return 0;
}

/** ----------------------------------------------------------- *
  *  rnds16                                                     *
  *                                                             *
  *  Generate crypto-secure 2 bytes random number.              *
  *                                                             *
  *  @param u16 [out] Random number.                            *
  *                                                             *
  *  @retval 0 Program ran without errors.                      *
  *  @retval 1 Program ran into an error.                       *
  * ----------------------------------------------------------- **/

int rnds16(uint16_t* const restrict u16)
{
  rand64 >>= 16;

  if (!(rand64 & 0xFF00)) {
    #ifdef __RDRND__
    _rdrand64_step((unsigned long long*)&rand64); 

    #elif defined(__ARM_FEATURE_RNG)
    __rndr((unsigned long long*)&rand64); // Might consider using rndrrs

    #else
    if (getrandom(&rand64, sizeof(uint64_t), 0) != sizeof(uint64_t))
      return 1;
    #endif
  }
  
  *u16 = (rand64 & 0xFFFF);

  return 0;
}

/** ----------------------------------------------------------- *
  *  rnds32                                                      *
  *                                                             *
  *  Generate crypto-secure 4 byte random number.               *
  *                                                             *
  *  @param u32 [out] Random number.                            *
  *                                                             *
  *  @retval 0 Program ran without errors.                      *
  *  @retval 1 Program ran into an error.                       *
  * ----------------------------------------------------------- **/

int rnds32(uint32_t* const restrict u32)
{
  rand64 >>= 32;

  if (!(rand64 & 0xFF000000)) {
    #ifdef __RDRND__
    _rdrand64_step((unsigned long long*)&rand64); 

    #elif defined(__ARM_FEATURE_RNG)
    __rndr((unsigned long long*)&rand64); // Might consider using rndrrs

    #else
    if (getrandom(&rand64, sizeof(uint64_t), 0) != sizeof(uint64_t))
      return 1;
    #endif
  }

  *u32 = (rand64 & 0xFFFFFFFF);
  
  return 0;
}

/** ----------------------------------------------------------- *
  *  rnds64                                                     *
  *                                                             *
  *  Generate crypto-secure 8 bytes random number.              *
  *                                                             *
  *  @param u64 [out] Random number.                            *
  *                                                             *
  *  @retval 0 Program ran without errors.                      *
  *  @retval 1 Program ran into an error.                       *
  * ----------------------------------------------------------- **/

int rnds64(uint64_t* const restrict u64)
{

  #ifdef __RDRND__
  _rdrand64_step((unsigned long long*)&rand64); 

  #elif defined(__ARM_FEATURE_RNG)
  __rndr((unsigned long long*)&rand64); // Might consider using rndrrs

  #else
  if (getrandom(&rand64, sizeof(uint64_t), 0) != sizeof(uint64_t))
    return 1;
  
  #endif

  *u64 = rand64;

  return 0;
}
