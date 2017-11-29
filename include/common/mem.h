
#pragma once

#include <new>
#include <memory>
#include <utility>
#include <type_traits>
#include <common/types.h>

#if _MSC_VER >= 1900
#define UTILS_DECLSPEC_ALLOCATOR __declspec(allocator)
#else
#define UTILS_DECLSPEC_ALLOCATOR
#endif

namespace mem {
namespace internal {

inline UTILS_DECLSPEC_ALLOCATOR
void* alloc(size_t count, size_t size)
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

inline void free(void* ptr, size_t count, size_t size)
{
	if (count > static_cast<size_t>(-1) / size)
	{
		throw ::std::bad_alloc();
	}
	return ::operator delete(ptr);
}

} // namespace internal

extern alloc_func g_alloc;
extern free_func g_free;

#define DEFINE_GLOBAL_ALLOC_FUNCTIONS \
namespace mem { \
alloc_func g_alloc = internal::alloc; \
free_func g_free = internal::free; \
}

inline void* __cdecl alloc(size_t count, size_t size)
{
	return internal::alloc(count, size);
}

inline void __cdecl free(void* ptr, size_t count, size_t size)
{
	return internal::free(ptr, count, size);
}


template <class T, class... Args>
inline UTILS_DECLSPEC_ALLOCATOR
T* placement_alloc(const alloc_func_not_null alloc, Args&&... args)
{
	T* ptr = static_cast<T*>(alloc(1, sizeof(T)));
	return ::new (ptr) T(::std::forward<Args>(args)...);
}

template <class T, class... Args>
inline UTILS_DECLSPEC_ALLOCATOR
T* placement_alloc(size_t count, const alloc_func_not_null alloc, Args&&... args)
{
	T* ptr = static_cast<T*>(alloc(count, sizeof(T)));
	while (count != 0) {
		T* ptrN = &ptr[--count];
		ptrN = ::new (ptrN) T(::std::forward<Args>(args)...);
	}
	return ptr;
}

template <class T>
inline void placement_free(T* ptr, const free_func_not_null free)
{
	ptr->~T();
	free(ptr, 1, sizeof(T));
}

template <class T>
inline void placement_free(T* ptr, size_t count, const free_func_not_null free)
{
	while (count != 0) {
		ptr[--count].~T();
	}
	free(ptr, count, sizeof(T));
}


template <class T, class... Args>
T* placement_g_alloc(Args&&... args)
{
	return placement_alloc<T>(g_alloc, ::std::forward<Args>(args)...);
}

template <class T, class... Args>
T* placement_g_alloc(size_t count, Args&&... args)
{
	return placement_alloc<T>(count, g_alloc, ::std::forward<Args>(args)...);
}

template <class T>
inline void placement_g_free(T* ptr)
{
	placement_free(ptr, g_free);
}

template <class T>
inline void placement_g_free(T* ptr, size_t count)
{
	placement_free(ptr, count, g_free);
}


class custom_free
{
public:
	constexpr custom_free(const free_func_not_null free) noexcept
		: m_free(free)
	{}
	free_func_not_null free() const noexcept {
		return m_free;
	}
private:
	const free_func_not_null m_free;
};

class regular_free
{
public:
	constexpr regular_free() = default;
	constexpr regular_free(const free_func_not_null) noexcept {}

	free_func_not_null free() const noexcept {
		return internal::free;
	}
};

class custom_allocator_functions
{
public:
	using free_type = custom_free;

	constexpr custom_allocator_functions(
		const alloc_func_not_null alloc,
		const free_func_not_null free) noexcept
		: m_alloc(alloc)
		, m_free(free)
	{}
	bool operator==(const custom_allocator_functions& other) const noexcept {
		return (m_alloc == other.m_alloc) && (m_free == other.m_free);
	}
	bool operator!=(const custom_allocator_functions& other) const noexcept {
		return !(*this == other);
	}
	alloc_func_not_null alloc() const noexcept {
		return m_alloc;
	}
	free_func_not_null free()  const noexcept {
		return m_free;
	}
private:
	const alloc_func_not_null m_alloc;
	const free_func_not_null m_free;
};

class regular_allocator_functions
{
public:
	using free_type = regular_free;

	bool operator==(const custom_allocator_functions&) const noexcept {
		return true;
	}
	bool operator!=(const custom_allocator_functions&) const noexcept {
		return false;
	}
	alloc_func_not_null alloc() const noexcept {
		return internal::alloc;
	}
	free_func_not_null free()  const noexcept {
		return internal::free;
	}
};

template <class Type, class Functions>
class customf_allocator
{
public:
	static_assert(!::std::is_const<Type>::value,
		"The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	using value_type = Type;
	using functions_type = Functions;
	using pointer = value_type*;
	using size_type = size_t;

	template<class, class> friend class customf_allocator;

	customf_allocator() noexcept = delete;

	explicit customf_allocator(const functions_type& allocFunctions) noexcept
		: m_allocFunctions(allocFunctions)
	{
	}

	customf_allocator(const customf_allocator&) noexcept = default;

	template <class Other>
	customf_allocator(const customf_allocator<Other, Functions>& other) noexcept
		: m_allocFunctions(other.m_allocFunctions)
	{
	}

	void deallocate(const pointer ptr, const size_type count)
	{
		(void)count;
		m_allocFunctions.free()(ptr, count, sizeof(Type));
	}

	UTILS_DECLSPEC_ALLOCATOR
	pointer allocate(const size_type count)
	{
		return static_cast<pointer>(m_allocFunctions.alloc()(count, sizeof(Type)));
	}

	template <class Other>
	bool operator==(const customf_allocator<Other, Functions>& other) noexcept
	{
		return m_allocFunctions == other.m_allocFunctions;
	}

	template <class Other>
	bool operator!=(const customf_allocator<Other, Functions>& other) noexcept
	{
		return m_allocFunctions != other.m_allocFunctions;
	}

private:
	functions_type m_allocFunctions;
};

template <class Type>
class globalf_allocator
{
public:
	static_assert(!::std::is_const<Type>::value,
		"The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	using value_type = Type;
	using pointer = value_type*;
	using size_type = size_t;

	globalf_allocator() noexcept = default;

	globalf_allocator(const globalf_allocator&) noexcept = default;

	template <class Other>
	globalf_allocator(const globalf_allocator<Other>&) noexcept
	{
	}

	void deallocate(const pointer ptr, const size_type count)
	{
		(void)count;
		g_free(ptr, count, sizeof(Type));
	}

	UTILS_DECLSPEC_ALLOCATOR
	pointer allocate(const size_type count)
	{
		return static_cast<pointer>(g_alloc(count, sizeof(Type)));
	}

	template <class Other>
	bool operator==(const globalf_allocator<Other>&) noexcept
	{
		return true;
	}

	template <class Other>
	bool operator!=(const globalf_allocator<Other>&) noexcept
	{
		return false;
	}
};

} // namespace mem

#undef UTILS_DECLSPEC_ALLOCATOR
