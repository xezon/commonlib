
#pragma once

namespace utils {

template <typename T, typename AllocFunc, typename... Args>
inline T* PlacementAlloc(AllocFunc alloc, Args&... args)
{
	T* p = static_cast<T*>(alloc(sizeof(T)));
	return new(p) T(args...);
}

template <typename T, typename FreeFunc>
inline void PlacementFree(T* p, FreeFunc free)
{
	p->~T();
	free(p);
}

template <typename T, typename AllocFunc, typename... Args>
inline T* PlacementAllocArray(size_t n, AllocFunc alloc, Args&... args)
{
	T* p = static_cast<T*>(alloc(n*sizeof(T)));
	while (n != 0) {
		T* pn = &p[--n];
		pn = new(pn) T(args...);
	}
	return p;
}

template <typename T, typename FreeFunc>
inline void PlacementFreeArray(T* p, size_t n, FreeFunc free)
{
	while (n != 0) {
		p[--n].~T();
	}
	free(p);
}

} // namespace utils
