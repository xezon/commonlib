
#pragma once

#include <new>
#include <memory>
#include <utility>
#include <type_traits>
#include "types.h"

#if _MSC_VER >= 1900
#define UTILS_DECLSPEC_ALLOCATOR __declspec(allocator)
#else
#define UTILS_DECLSPEC_ALLOCATOR
#endif

namespace utils {

inline UTILS_DECLSPEC_ALLOCATOR
void* __cdecl Alloc(size_t count, size_t size)
{
	if (count == 0)
	{
		return nullptr;
	}
	if (static_cast<size_t>(-1) / size < count)
	{
		throw ::std::bad_alloc();
	}
	return ::operator new(count * size);
}


inline void __cdecl Free(void* ptr, size_t count, size_t size)
{
	if (count > static_cast<size_t>(-1) / size)
	{
		throw ::std::bad_alloc();
	}
	return ::operator delete(ptr);
}

template <class T, class... Args>
inline UTILS_DECLSPEC_ALLOCATOR
T* PlacementAlloc(alloc_func_not_null alloc, Args&&... args)
{
	T* ptr = static_cast<T*>(alloc(1, sizeof(T)));
	return ::new (ptr) T(::std::forward<Args>(args)...);
}

template <class T>
inline void PlacementFree(T* ptr, free_func_not_null free)
{
	ptr->~T();
	free(ptr, 1, sizeof(T));
}

template <class T, class... Args>
inline UTILS_DECLSPEC_ALLOCATOR
T* PlacementAlloc(size_t count, alloc_func_not_null alloc, Args&&... args)
{
	T* ptr = static_cast<T*>(alloc(count, sizeof(T)));
	while (count != 0) {
		T* ptrN = &ptr[--count];
		ptrN = ::new (ptrN) T(::std::forward<Args>(args)...);
	}
	return ptr;
}

template <class T>
inline void PlacementFree(T* ptr, size_t count, free_func_not_null free)
{
	while (count != 0) {
		ptr[--count].~T();
	}
	free(ptr, count, sizeof(T));
}

struct SAllocatorFunctions
{
	constexpr SAllocatorFunctions(
		const alloc_func_not_null alloc,
		const free_func_not_null free) noexcept
		: alloc(alloc)
		, free(free)
	{}

	const alloc_func_not_null alloc;
	const free_func_not_null free;
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
		m_functions.free(ptr, count, sizeof(Type));
	}

	UTILS_DECLSPEC_ALLOCATOR
		pointer allocate(const size_type count)
	{
		return static_cast<pointer>(m_functions.alloc(count, sizeof(Type)));
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
