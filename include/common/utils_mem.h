
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
void* __cdecl alloc(size_t count, size_t size)
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


inline void __cdecl free(void* ptr, size_t count, size_t size)
{
	if (count > static_cast<size_t>(-1) / size)
	{
		throw ::std::bad_alloc();
	}
	return ::operator delete(ptr);
}

template <class T, class... Args>
inline UTILS_DECLSPEC_ALLOCATOR
T* placement_alloc(alloc_func_not_null alloc, Args&&... args)
{
	T* ptr = static_cast<T*>(alloc(1, sizeof(T)));
	return ::new (ptr) T(::std::forward<Args>(args)...);
}

template <class T, class... Args>
inline UTILS_DECLSPEC_ALLOCATOR
T* placement_alloc(size_t count, alloc_func_not_null alloc, Args&&... args)
{
	T* ptr = static_cast<T*>(alloc(count, sizeof(T)));
	while (count != 0) {
		T* ptrN = &ptr[--count];
		ptrN = ::new (ptrN) T(::std::forward<Args>(args)...);
	}
	return ptr;
}

template <class T>
inline void placement_free(T* ptr, free_func_not_null free)
{
	ptr->~T();
	free(ptr, 1, sizeof(T));
}

template <class T>
inline void placement_free(T* ptr, size_t count, free_func_not_null free)
{
	while (count != 0) {
		ptr[--count].~T();
	}
	free(ptr, count, sizeof(T));
}

struct custom_allocator_functions
{
	constexpr custom_allocator_functions(
		const alloc_func_not_null alloc,
		const free_func_not_null free) noexcept
		: alloc(alloc)
		, free(free)
	{}

	bool operator==(const custom_allocator_functions& other) noexcept {
		return (alloc == other.alloc) && (free == other.free);
	}
	bool operator!=(const custom_allocator_functions& other) noexcept {
		return !(*this == other);
	}

	const alloc_func_not_null alloc;
	const free_func_not_null free;
};

template <class Type>
class custom_allocator
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

	template<class> friend class custom_allocator;

	template<class Other>
	struct rebind
	{
		using other = custom_allocator<Other>;
	};

	pointer address(reference val) const noexcept
	{
		return ::std::addressof(val);
	}

	const_pointer address(const_reference val) const noexcept
	{
		return ::std::addressof(val);
	}

	custom_allocator() noexcept = delete;

	explicit custom_allocator(const custom_allocator_functions& allocfunc) noexcept
		: m_allocfunc(allocfunc)
	{
	}

	custom_allocator(const custom_allocator&) noexcept = default;

	template <class Other>
	custom_allocator(const custom_allocator<Other>& other) noexcept
		: m_allocfunc(other.m_allocfunc)
	{
	}

	void deallocate(const pointer ptr, const size_type count)
	{
		(void)count;
		m_allocfunc.free(ptr, count, sizeof(Type));
	}

	UTILS_DECLSPEC_ALLOCATOR
		pointer allocate(const size_type count)
	{
		return static_cast<pointer>(m_allocfunc.alloc(count, sizeof(Type)));
	}

	UTILS_DECLSPEC_ALLOCATOR
		pointer allocate(const size_type count, const void*)
	{
		return allocate(count);
	}

	template <class ObjType, class... Args>
	void construct(ObjType* const ptr, Args&&... args)
	{
		::new (const_cast<void *>(static_cast<const volatile void*>(ptr)))
			ObjType(::std::forward<Args>(args)...);
	}

	template <class ObjType>
	void destroy(ObjType* const ptr)
	{
		ptr->~ObjType();
	}

	size_t max_size() const noexcept
	{
		return (static_cast<size_t>(-1) / sizeof(Type));
	}

	template <class Other>
	bool operator==(const custom_allocator<Other>& other) noexcept
	{
		return m_allocfunc == other.m_allocfunc;
	}

	template <class Other>
	bool operator!=(const custom_allocator<Other>& other) noexcept
	{
		return m_allocfunc != other.m_allocfunc;
	}

private:
	custom_allocator_functions m_allocfunc;
};

} // namespace utils

#undef UTILS_DECLSPEC_ALLOCATOR
