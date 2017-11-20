
#pragma once

#include <common/stl.h>
#include <iostream>

namespace util {

template <class... Args>
inline void clog(const char* format, Args... args)
{
	::std::clog << ::stl::string_format(format, args...).c_str() << ::std::endl;
}

template <class... Args>
inline void wclog(const wchar_t* format, Args... args)
{
	::std::wclog << ::stl::wstring_format(format, args...).c_str() << ::std::endl;
}

template <class... Args>
inline void cerr(const char* format, Args... args)
{
	::std::cerr << ::stl::string_format(format, args...).c_str() << ::std::endl;
}

template <class... Args>
inline void wcerr(const wchar_t* format, Args... args)
{
	::std::wcerr << ::stl::wstring_format(format, args...).c_str() << ::std::endl;
}

} // namespace util
