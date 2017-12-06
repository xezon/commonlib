
#pragma once

#include <codecvt>
#include <string>

namespace utf8 {

inline ::std::u16string to_utf16(const ::std::string& str)
{
	::std::wstring_convert<::std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.from_bytes(str);
}

inline ::std::u16string to_utf16(const char* str)
{
	::std::wstring_convert<::std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.from_bytes(str);
}

inline ::std::u16string to_utf16(const char* begin, const char* end)
{
	::std::wstring_convert<::std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.from_bytes(begin, end);
}

inline ::std::wstring to_ucs2(const ::std::string& str)
{
	::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.from_bytes(str);
}

inline ::std::wstring to_ucs2(const char* str)
{
	::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.from_bytes(str);
}

inline ::std::wstring to_ucs2(const char* begin, const char* end)
{
	::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.from_bytes(begin, end);
}

inline ::std::string to_utf8(const ::std::u16string& str)
{
	::std::wstring_convert<::std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.to_bytes(str);
}

inline ::std::string to_utf8(const char16_t* str)
{
	::std::wstring_convert<::std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.to_bytes(str);
}

inline ::std::string to_utf8(const char16_t* begin, const char16_t* end)
{
	::std::wstring_convert<::std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.to_bytes(begin, end);
}

inline ::std::string to_utf8(const ::std::wstring& str)
{
	::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.to_bytes(str);
}

inline ::std::string to_utf8(const wchar_t* str)
{
	::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.to_bytes(str);
}

inline ::std::string to_utf8(const wchar_t* begin, const wchar_t* end)
{
	::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.to_bytes(begin, end);
}

// Intentional string pass-through for platform dependent string types

inline ::std::string to_utf8(const ::std::string& str)
{
	return str;
}

inline ::std::string to_utf8(const char* str)
{
	return str;
}

} // namespace utf8
