#ifndef SHORTTYPES_H
#define SHORTTYPES_H

#include <ctype.h>
#include <stdint.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float       f32;
/* IEEE 754 assures 64 bit doubles*/
#ifdef __STDC_IEC_559__
typedef long double f64;
#else
#warning "f64 type disabled no IEEE 754 floating point"
#endif

#define global static
#define persist static
#define internal static

#endif //!SHORTTYPES_H
