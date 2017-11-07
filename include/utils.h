
#pragma once

#include <chrono>
#include <thread>
#include <fstream>

using namespace std::chrono_literals;

namespace utils
{

template<typename T, unsigned int size>
inline unsigned int GetArraySize(T(&)[size])
{
	return size;
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

} // namespace utils
