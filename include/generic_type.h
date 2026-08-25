/**
 * @file  generic_type.h
 * @brief Generic type definitions
 *
 * @author Tom Schmitt
 *
 * Copyright (c) 2026 Tom Schmitt
 * All rights reserved.
 *
 */

#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#ifdef FLT16_MAX
    #if defined(__SSE2__)
    typedef _Float16    float16_t;
    #else
    typedef float       float16_t; // Dirty workaround, might be updated later
    #endif
#endif

typedef float       float32_t;
typedef double      float64_t;
typedef long double float80_t; // Please make sure you compile with gcc, else it wont be 80 bits

#endif
