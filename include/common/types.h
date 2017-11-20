
#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#ifdef ZORRO_LITE_C

typedef char           int8_t;
typedef short          int16_t;
typedef int            int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef uint32_t       size_t;

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
#ifdef MQL4

#define int8_t   char
#define int16_t  short
#define int32_t  int
#define int64_t  long
#define uint32_t uint
#define uint8_t  uchar
#define uint16_t ushort
#define uint64_t ulong
#define size_t   uint32_t

typedef void* (* alloc_func)(size_t count, size_t size);
typedef void  (* free_func )(void* ptr, size_t count, size_t size);

#else

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif // __cplusplus

typedef void* (__cdecl* alloc_func)(size_t count, size_t size);
typedef void  (__cdecl* free_func )(void* ptr, size_t count, size_t size);

#ifdef __cplusplus
#include <gsl/pointers>
typedef ::gsl::not_null<alloc_func> alloc_func_not_null;
typedef ::gsl::not_null<free_func>  free_func_not_null;
#endif // __cplusplus

#endif // MQL4
#endif // ZORRO_LITE_C

#endif // COMMON_TYPES_H_
