
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

inline void* __cdecl Alloc(size_t size)
{
	return malloc(size);
}

inline void __cdecl Free(void* p)
{
	free(p);
}

#endif // COMMON_UTILSC_H_
