#pragma once

#ifndef forceinline
    #define forceinline __attribute__((always_inline)) inline
#endif // forceinline

// Hard-coded for now
#if (!defined(__BYTE_ORDER__) || !defined(__ORDER_LITTLE_ENDIAN__) || !defined(__ORDER_BIG_ENDIAN__))
    #error "Need __BYTE_ORDER__, __ORDER_LITTLE_ENDIAN__ and __ORDER_BIG_ENDIAN__ to be defined"
#endif

#if (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__ && __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    #error "Host must be either big- or little-endian"
#endif

#ifdef _WIN32
    #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
        #error "Need Windows 32-bit platform to be little-endian!"
    #endif
#endif // _WIN32

#include <misc/rio_Types.h>

#define register
