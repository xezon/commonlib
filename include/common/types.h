
#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#ifdef ZORRO_LITE_C

typedef char           int8_t;
typedef short          int16_t;
typedef int            int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef struct sint64
{
	uint32_t lo;
	int32_t hi;
} int64_t;

typedef struct uint64
{
	uint32_t lo;
	uint32_t hi;
} uint64_t;

typedef void* alloc_func;
typedef void* free_func;

#else

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif // __cplusplus

typedef void* (__cdecl* alloc_func)(size_t size);
typedef void  (__cdecl* free_func )(void* p);

#endif // ZORRO_LITE_C

#endif // COMMON_TYPES_H_
