
#pragma once

#include <chrono>
#include <thread>
#include <fstream>
#include <cassert>
#include <common/types.h>

#define UTILS_DELETE_COPY_CONSTRUCTOR(clazz) \
	clazz(const clazz&) = delete; \
	clazz& operator=(const clazz&) = delete;

using namespace ::std::chrono_literals;

namespace util {

template <class Char>
inline bool is_valid_string(Char* str)
{
	return str && *str;
}

inline const char* get_valid_string(const char* str)
{
	return (str != 0) ? str : "";
}

inline const wchar_t* get_valid_string(const wchar_t* str)
{
	return (str != 0) ? str : L"";
}

template <class Type, size_t Size>
inline size_t array_size(Type(&)[Size])
{
	return Size;
}

inline bool file_exists(const wchar_t* filename)
{
	if (filename && filename[0])
	{
		::std::ifstream file(filename);
		return file.good();
	}
	return false;
}

template <class Time>
inline void sleep(Time time)
{
	::std::this_thread::sleep_for(time);
}

template <class Type>
inline void verify_initialized_pointers_debug(const Type& object)
{
	(void)object;
#ifdef _DEBUG
	const size_t size = sizeof(Type) / sizeof(::std::uintptr_t);
	for (int i = 0; i < size; ++i)
	{
		auto ptr = reinterpret_cast<const ::std::uintptr_t*>(&object) + i;
		assert(*ptr != 0u);
	}
#endif
}

template <class TypeA, class TypeB>
inline void verify_equal_pointers_debug(TypeA* pA, TypeB* pB)
{
	(void)pA;
	(void)pB;
#ifdef _DEBUG
	assert(reinterpret_cast<const void*>(pA) == reinterpret_cast<const void*>(pB));
#endif
}

template <class Type>
inline void nullify_object(Type& object)
{
	::memset(&object, 0, sizeof(Type));
}

template <class Type>
inline void nullify_object_debug(Type& object)
{
	(void)object;
#ifdef _DEBUG
	nullify_object(object);
#endif
}

} // namespace util
