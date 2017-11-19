
#pragma once

#include "types.h"
#include "utils_c.h"
#include "utils_mem.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <cassert>

#define UTILS_DELETE_COPY_CONSTRUCTOR(clazz) \
	clazz(const clazz&) = delete; \
	clazz& operator=(const clazz&) = delete;

using namespace ::std::chrono_literals;

namespace utils {

template <typename T, size_t Size>
inline size_t GetArraySize(T(&)[Size])
{
	return Size;
}

inline bool FileExists(const wchar_t* filename)
{
	if (filename && filename[0])
	{
		::std::ifstream file(filename);
		return file.good();
	}
	return false;
}

template <typename T>
inline void Sleep(T time)
{
	::std::this_thread::sleep_for(time);
}

template <class T>
inline void VerifyFunctionsStruct(const T& instance)
{
	(void)instance;
#ifdef _DEBUG
	const size_t size = sizeof(T) / sizeof(::std::uintptr_t);
	for (int i = 0; i < size; ++i)
	{
		auto ptr = reinterpret_cast<const ::std::uintptr_t*>(&instance) + i;
		assert(*ptr != 0);
	}
#endif
}

template <class TypeA, class TypeB>
inline void VerifyEqualPointers(TypeA* pA, TypeB* pB)
{
	assert(reinterpret_cast<const void*>(pA) == reinterpret_cast<const void*>(pB));
}

} // namespace utils
