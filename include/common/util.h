
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

const char s_lower_ascii_table[256] = 
{
	'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
	'\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
	'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
	'\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
	'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
	'\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
	'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
	'\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
	'\x40', 'a',    'b',    'c',    'd',    'e',    'f',    'g',
	'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
	'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
	'x',    'y',    'z',    '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
	'\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
	'\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
	'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
	'\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
	'\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
	'\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
	'\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
	'\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
	'\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7',
	'\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
	'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7',
	'\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
	'\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7',
	'\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
	'\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7',
	'\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
	'\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7',
	'\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
	'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7',
	'\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};

inline unsigned char to_lower_ascii(unsigned char ch)
{
	return static_cast<unsigned char>(s_lower_ascii_table[ch]);
}

inline char to_lower_ascii(char ch)
{
	return s_lower_ascii_table[static_cast<unsigned char>(ch)];
}

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

template <class Type>
inline void SafeRelease(Type*& p) {
	if (p) {
		p->Release();
		p = 0;
	}
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
