#ifdef INCLUDE_CAFE
#include <cafe.h>
#else
#include <types.h>
#endif // INCLUDE_CAFE

#ifndef EFT_TYPES_H_
#define EFT_TYPES_H_

#ifndef EFT_WIN

#ifndef forceinline
#define forceinline __attribute__((always_inline)) inline
#endif // forceinline

// https://stackoverflow.com/a/1597129
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define static_assert(condition, ...) typedef int TOKENPASTE2(static_assert_, __LINE__)[(condition) ? 1 : -1]

#endif // EFT_WIN

#endif // EFT_TYPES_H_
