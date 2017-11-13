
#pragma once

#include <Windows.h>

namespace utils {

template<typename F>
inline F GetProcAddressT(HMODULE hModule, LPCSTR lpProcName)
{
	return reinterpret_cast<F>(::GetProcAddress(hModule, lpProcName));
}

inline void SafeCloseHandle(HANDLE& handle)
{
	if (handle != HANDLE(0))
	{
		::CloseHandle(handle);
		handle = 0;
	}
}

} // namespace utils
