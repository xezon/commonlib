
#pragma once

#include <utility>
#include <gsl/pointers>
#include "types.h"

#if _MSC_VER >= 1900
#define UTILS_DECLSPEC_ALLOCATOR __declspec(allocator)
#else
#define UTILS_DECLSPEC_ALLOCATOR
#endif

namespace utils {

template <typename T, typename AllocFunc, typename... Args>
UTILS_DECLSPEC_ALLOCATOR
inline T* PlacementAlloc(AllocFunc alloc, Args&&... args)
{
	T* p = static_cast<T*>(alloc(sizeof(T)));
	return ::new (p) T(::std::forward<Args>(args)...);
}

template <typename T, typename FreeFunc>
inline void PlacementFree(T* p, FreeFunc free)
{
	p->~T();
	free(p);
}

template <typename T, typename AllocFunc, typename... Args>
UTILS_DECLSPEC_ALLOCATOR
inline T* PlacementAllocArray(size_t n, AllocFunc alloc, Args&&... args)
{
	T* p = static_cast<T*>(alloc(n*sizeof(T)));
	while (n != 0) {
		T* pn = &p[--n];
		pn = ::new (pn) T(::std::forward<Args>(args)...);
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

struct SAllocatorFunctions
{
	using TAllocFunc = ::gsl::not_null<alloc_func>;
	using TFreeFunc = ::gsl::not_null<free_func>;

	constexpr SAllocatorFunctions(const TAllocFunc alloc, const TFreeFunc free) noexcept
		: alloc(alloc)
		, free(free)
	{}

	const TAllocFunc alloc;
	const TFreeFunc free;
};

template<class Type>
class CAllocatorWithFunc
{
public:
	static_assert(!::std::is_const<Type>::value,
		"The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	using value_type = Type;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using propagate_on_container_move_assignment = ::std::true_type;
	using is_always_equal = ::std::true_type;

	template<class> friend class CAllocatorWithFunc;

	template<class Other>
	struct rebind
	{
		using other = CAllocatorWithFunc<Other>;
	};

	pointer address(reference _Val) const noexcept
	{
		return ::std::addressof(_Val);
	}

	const_pointer address(const_reference _Val) const noexcept
	{
		return ::std::addressof(_Val);
	}

	CAllocatorWithFunc() noexcept = delete;

	explicit CAllocatorWithFunc(const SAllocatorFunctions& functions) noexcept
		: m_functions(functions)
	{
	}

	CAllocatorWithFunc(const CAllocatorWithFunc&) noexcept = default;

	template<class Other>
	CAllocatorWithFunc(const CAllocatorWithFunc<Other>& other) noexcept
		: m_functions(other.m_functions)
	{
	}

	void deallocate(const pointer ptr, const size_type count)
	{
		(void)count;
		m_functions.free(ptr);
	}

	UTILS_DECLSPEC_ALLOCATOR
		pointer allocate(const size_type count)
	{
		return (static_cast<pointer>(m_functions.alloc(count * sizeof(Type))));
	}

	UTILS_DECLSPEC_ALLOCATOR
		pointer allocate(const size_type count, const void*)
	{
		return allocate(count);
	}

	template<class ObjType, class... Args>
	void construct(ObjType* const ptr, Args&&... args)
	{
		::new (const_cast<void *>(static_cast<const volatile void*>(ptr)))
			ObjType(::std::forward<Args>(args)...);
	}

	template<class ObjType>
	void destroy(ObjType* const ptr)
	{
		ptr->~ObjType();
	}

	size_t max_size() const noexcept
	{
		return (static_cast<size_t>(-1) / sizeof(Type));
	}

private:
	SAllocatorFunctions m_functions;
};

} // namespace utils

#undef UTILS_DECLSPEC_ALLOCATOR
