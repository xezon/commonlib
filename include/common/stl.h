
#pragma once

#include <string>
#include <cstdio>
#include <memory>
#include <algorithm>

namespace stl {

template <size_t Size, class... Args>
inline ::std::string string_format_n(const char* format, Args... args)
{
	char buf[Size];
	::std::snprintf(buf, Size, format, args...);
	return ::std::string(buf, buf + Size);
}

template <size_t Size, class... Args>
inline ::std::wstring wstring_format_n(const wchar_t* format, Args... args)
{
	wchar_t buf[Size];
	::std::swprintf(buf, Size, format, args...);
	return ::std::wstring(buf, buf + Size);
}

template <class... Args>
inline ::std::string string_format(const char* format, Args... args)
{
	return string_format_n<1024>(format, args...);
}

template <class... Args>
inline ::std::wstring wstring_format(const wchar_t* format, Args... args)
{
	return wstring_format_n<1024>(format, args...);
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

} // namespace stl
