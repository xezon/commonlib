
#pragma once

#include "stl.h"
#include <iostream>

namespace utils
{

template<typename... Args>
inline void LogA(const char* format, Args... args)
{
	::std::clog << ::stl::string_format(format, std::forward<Args>(args)...).c_str() << ::std::endl;
}

template<typename... Args>
inline void LogW(const wchar_t* format, Args... args)
{
	::std::wclog << ::stl::wstring_format(format, std::forward<Args>(args)...).c_str() << ::std::endl;
}

template<typename... Args>
inline void ErrA(const char* format, Args... args)
{
	::std::cerr << ::stl::string_format(format, std::forward<Args>(args)...).c_str() << ::std::endl;
}

template<typename... Args>
inline void ErrW(const wchar_t* format, Args... args)
{
	::std::wcerr << ::stl::wstring_format(format, std::forward<Args>(args)...).c_str() << ::std::endl;
}

}
