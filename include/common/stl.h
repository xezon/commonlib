
#pragma once

#include <string>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <exception>

namespace stl {

template <class... Args>
inline int nprintf(char* buf, size_t count, const char* format, Args... args)
{
	return ::std::snprintf(buf, count, format, args...);
}

template <class... Args>
inline int nprintf(wchar_t* buf, size_t count, const wchar_t* format, Args... args)
{
	return ::std::swprintf(buf, count, format, args...);
}

template <class String, size_t Size, class... Args>
inline String string_format_tn(const typename String::allocator_type& allocator,
                               const typename String::value_type* format, Args... args)
{
	typename String::value_type buf[Size];
	const int n = ::stl::nprintf(buf, Size, format, args...);
	if (n >= Size) {
		throw ::std::length_error("Size is too small");
	}
	if (n < 0) {
		throw ::std::invalid_argument("Encoding error occured");
	}
	String str(allocator);
	str.assign(buf, buf + Size);
	return str;
}

template <class String, class... Args>
inline String string_format_t(const typename String::allocator_type& allocator,
                              const typename String::value_type* format, Args... args)
{
	return string_format_tn<String, 1024>(allocator, format, args...);
}

template <size_t Size, class... Args>
inline ::std::string string_format_n(const char* format, Args... args)
{
	return string_format_tn<::std::string, Size>(::std::string::allocator_type(), format, args...);
}

template <class... Args>
inline ::std::string string_format(const char* format, Args... args)
{
	return string_format_t<::std::string>(::std::string::allocator_type(), format, args...);
}

template <size_t Size, class... Args>
inline ::std::wstring wstring_format_n(const wchar_t* format, Args... args)
{
	return string_format_tn<::std::wstring, Size>(::std::wstring::allocator_type(), format, args...);
}

template <class... Args>
inline ::std::wstring wstring_format(const wchar_t* format, Args... args)
{
	return string_format_t<::std::wstring>(::std::wstring::allocator_type(), format, args...);
}

template <class Container, class Value>
inline bool find_and_erase(Container& container, const Value& value)
{
	typename Container::iterator it = ::std::find(container.begin(), container.end(), value);
	if (it != container.end())
	{
		container.erase(it);
		return true;
	}
	return false;
}

template <class Container, class Predicate>
inline bool find_and_erase_if(Container& container, const Predicate& predicate)
{
	typename Container::iterator it = ::std::find_if(container.begin(), container.end(), predicate);
	if (it != container.end())
	{
		container.erase(it);
		return true;
	}
	return false;
}

template <class Iterator, class Value, class Compare = ::std::less<>>
inline Iterator binary_find(Iterator first, Iterator last, const Value& value, Compare comp = {})
{
	first = ::std::lower_bound(first, last, value, comp);
	return (first != last) && !comp(value, *first) ? first : last;
}

template <class Iterator, class Value, class Compare = ::std::less<>>
inline typename Iterator::difference_type binary_find_index(Iterator first, Iterator last, const Value& value, Compare comp = {})
{
	Iterator it = binary_find(first, last, value, comp);
	return ::std::distance(first, it);
}

template <class Type>
inline void clear_mem(Type& object)
{
	Type().swap(object);
}

} // namespace stl
