
#ifndef COMMON_UTILSC_H_
#define COMMON_UTILSC_H_

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

inline int IsValidString(const char* str)
{
	return str && *str;
}

inline int IsValidWString(const wchar_t* str)
{
	return str && *str;
}

inline void* __cdecl Cmalloc(size_t count, size_t size)
{
	return malloc(count * size);
}

inline void __cdecl Cfree(void* ptr, size_t count, size_t size)
{
	(void)count;
	(void)size;
	free(ptr);
}

#endif // COMMON_UTILSC_H_
