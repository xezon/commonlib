
#ifndef COMMON_UTILSC_H_
#define COMMON_UTILSC_H_

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

inline int is_valid_string(const char* str)
{
	return str && *str;
}

inline int is_valid_wstring(const wchar_t* str)
{
	return str && *str;
}

inline const char* get_valid_string(const char* str)
{
	return (str != 0) ? str : "";
}

inline const wchar_t* get_valid_wstring(const wchar_t* str)
{
	return (str != 0) ? str : L"";
}

inline void* __cdecl c_malloc(size_t count, size_t size)
{
	return malloc(count * size);
}

inline void __cdecl c_free(void* ptr, size_t count, size_t size)
{
	(void)count;
	(void)size;
	free(ptr);
}

#endif // COMMON_UTILSC_H_
