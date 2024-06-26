#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#ifndef __cplusplus
typedef u8 bool;
#  define true 1
#  define false 0
#  define nullptr ((void*) 0)
#endif

#define null nullptr

#define attribute(attr) __attribute__((attr))

#endif //TYPES_H
