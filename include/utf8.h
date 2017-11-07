
#pragma once

#include <codecvt>
#include <string>

namespace utf8
{
inline std::u16string to_utf16(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.from_bytes(str);
}

inline std::u16string to_utf16(const char* str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.from_bytes(str);
}

inline std::wstring to_ucs2(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.from_bytes(str);
}

inline std::wstring to_ucs2(const char* str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.from_bytes(str);
}

inline std::string to_utf8(std::u16string str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.to_bytes(str);
}

inline std::string to_utf8(const char16_t* str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> c;
	return c.to_bytes(str);
}

inline std::string to_utf8(std::wstring str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.to_bytes(str);
}

inline std::string to_utf8(const wchar_t* str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> c;
	return c.to_bytes(str);
}

} // namespace utf8
