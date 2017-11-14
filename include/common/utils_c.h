
#ifndef UTILS_C_H
#define UTILS_C_H

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

inline bool IsValidString(const char* str)
{
	return str && *str;
}

inline bool IsValidString(const wchar_t* str)
{
	return str && *str;
}

inline void* __cdecl Alloc(size_t size)
{
	return ::malloc(size);
}

inline void __cdecl Free(void* p)
{
	::free(p);
}

#endif // UTILS_C_H
