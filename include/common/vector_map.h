// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

#ifndef __SORTEDVECTOR_H__
#define __SORTEDVECTOR_H__

#include <vector>
#include <functional>

//! --------------------------------------------------------------------------
//! VectorMap
//! Usage Notes:
//! This class is designed to be an (almost, see below) drop-in replacement
//! for std::map. It features an almost identical interface, but it is
//! implemented using a sorted vector rather than a tree. This is in most
//! cases more efficient, as there is less dynamic memory allocation and
//! pointer dereferencing.
//! *************************************************************************
//! PLEASE NOTE: There is one vital difference between std::map and VectorMap
//! that you will need to note before trying to replace std::map. Since
//! VectorMap is implemented using a vector, iterators can and will be
//! invalidated by many operations, such as insertions and deletions, and
//! due to sorting potentially even normal lookups. Please Please PLEASE make
//! sure that you are not storing any iterators to this class.
//! *************************************************************************
//! The class varies from the std::set API in that two of the erase methods
//! methods are not of void return type but return an iterator - this is
//! required in practice because they invalidate iterators, as noted above.
//! * iterator erase(iterator where);
//! * iterator erase(iterator first, iterator last);
//! Performance Notes:
//! This class uses the empty base optimization hack to allow comparison
//! predicate objects that have no state to take up no space in the object.
//! As a result the size of the overall VectorMap instance is the same as
//! that of the std::vector it uses to store the elements.
//! In addition to the normal map interface, this class provides the
//! following members that can be used to manage memory requirements:
//! * void reserve(size_type count);
//! Allocate enough space for count elements (see vector::reserve()).
//! * size_type capacity() const;
//! Report how many elements can be stored without reallocating (see
//! vector::capacity()).
//! --------------------------------------------------------------------------
template<typename K, typename V, typename T = std::less<K>, typename A = std::allocator<std::pair<const K, V>>>
class vector_map : private T // Empty base optimization
{
public:
	typedef K                                           key_type;
	typedef V                                           mapped_type;
	typedef A                                           allocator_type;

	typedef std::pair<const key_type, mapped_type> value_type;

	// For std::map the std::pair key value should be const however we are emulating a map in an std::vector
	// as we need functions such as sort() on the internal vector it cannot be const.
	// For complete safety one would need to add a iterator wrapper, wrapping the none-const type to a const type, preventing the use to get access to the non-const key.
	typedef std::pair<key_type, mapped_type> none_const_value_type; 
	typedef typename A::template rebind<none_const_value_type>::other none_const_allocator_type;

	typedef T                                           key_compare;

	class FirstLess
	{
	public:
		FirstLess(const key_compare& comp) : m_comp(comp) {}

		bool operator()(const value_type& left, const value_type& right) const
		{
			return m_comp(left.first, right.first);
		}

	private:
		const key_compare& m_comp;
	};

	typedef std::vector<none_const_value_type, none_const_allocator_type>         container_type;
	typedef typename container_type::iterator               iterator;
	typedef typename container_type::const_iterator         const_iterator;
	typedef typename container_type::reverse_iterator       reverse_iterator;
	typedef typename container_type::const_reverse_iterator const_reverse_iterator;
	typedef value_type&                                     reference;
	typedef const value_type&                               const_reference;
	typedef value_type*                                     pointer;
	typedef const value_type*                               const_pointer;
	typedef typename allocator_type::size_type              size_type;

	vector_map();
	explicit vector_map(const key_compare& comp);
	explicit vector_map(const key_compare& comp, const allocator_type& alloc);
	vector_map(const vector_map& right);
	template<class InputIterator> vector_map(InputIterator first, InputIterator last);
	template<class InputIterator> vector_map(InputIterator first, InputIterator last, const key_compare& comp);
	template<class InputIterator> vector_map(InputIterator first, InputIterator last, const key_compare& comp, const allocator_type& alloc);
	void                                      SwapElementsWithVector(container_type& elementVector);
	iterator                                  begin();
	const_iterator                            begin() const;
	size_type                                 capacity() const;
	void                                      clear();
	void                                      clearAndFreeMemory();
	size_type                                 count(const key_type& key) const;
	bool                                      empty() const;
	iterator                                  end();
	const_iterator                            end() const;
	std::pair<iterator, iterator>             equal_range(const key_type& key);
	std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;
	iterator                                  erase(iterator where);                //!< See documentation above.
	iterator                                  erase(iterator first, iterator last); //!< See documentation above.
	void                                      erase(const key_type& key);
	template<typename Predicate> void         erase_if(const Predicate& predicate);
	iterator                                  find(const key_type& key);
	const_iterator                            find(const key_type& key) const;
	allocator_type                            get_allocator() const;
	std::pair<iterator, bool>                 insert(const value_type& val);
	iterator                                  insert(iterator where, const value_type& val);
	template<class InputIterator> void        insert(InputIterator first, InputIterator last);
	key_compare                               key_comp() const;
	iterator                                  lower_bound(const key_type& key);
	const_iterator                            lower_bound(const key_type& key) const;
	size_type                                 max_size() const;
	reverse_iterator                          rbegin();
	const_reverse_iterator                    rbegin() const;
	reverse_iterator                          rend();
	const_reverse_iterator                    rend() const;
	void                                      reserve(size_type count);
	size_type                                 size() const;
	void                                      swap(vector_map& other);
	iterator                                  upper_bound(const key_type& key);
	const_iterator                            upper_bound(const key_type& key) const;
	mapped_type&                              operator[](const key_type& key);

	template<typename Sizer>
	void GetMemoryUsage(Sizer* pSizer) const
	{
		pSizer->AddObject(m_entries);
	}
private:
	container_type m_entries;
};

template<typename K, typename V, typename T, typename A>
vector_map<K, V, T, A>::vector_map()
{
}

template<typename K, typename V, typename T, typename A>
vector_map<K, V, T, A>::vector_map(const key_compare& comp)
	: key_compare(comp)
{
}

template<typename K, typename V, typename T, typename A>
vector_map<K, V, T, A>::vector_map(const key_compare& comp, const allocator_type& alloc)
	: key_compare(comp),
	m_entries(alloc)
{
}

template<typename K, typename V, typename T, typename A>
vector_map<K, V, T, A>::vector_map(const vector_map& right)
	: key_compare(right),
	m_entries(right.m_entries)
{
}

template<typename K, typename V, typename T, typename A>
template<class InputIterator> vector_map<K, V, T, A>::vector_map(InputIterator first, InputIterator last)
{
	for (; first != last; ++first)
		m_entries.push_back(*first);
	std::sort(m_entries.begin(), m_entries.end(), FirstLess(static_cast<key_compare>(*this)));
}

template<typename K, typename V, typename T, typename A>
template<class InputIterator> vector_map<K, V, T, A>::vector_map(InputIterator first, InputIterator last, const key_compare& comp)
	: key_compare(comp)
{
	for (; first != last; ++first)
		m_entries.push_back(*first);
	std::sort(m_entries.begin(), m_entries.end(), FirstLess(static_cast<key_compare>(*this)));
}

template<typename K, typename V, typename T, typename A>
template<class InputIterator> vector_map<K, V, T, A>::vector_map(InputIterator first, InputIterator last, const key_compare& comp, const allocator_type& alloc)
	: key_compare(comp),
	m_entries(alloc)
{
	for (; first != last; ++first)
		m_entries.push_back(*first);
	std::sort(m_entries.begin(), m_entries.end(), FirstLess(static_cast<key_compare>(*this)));
}

template<typename K, typename V, typename T, typename A>
void vector_map<K, V, T, A >::SwapElementsWithVector(typename vector_map<K, V, T, A>::container_type& elementVector)
{
	m_entries.swap(elementVector);
	std::sort(m_entries.begin(), m_entries.end(), FirstLess(static_cast<key_compare>(*this)));
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::begin()
{
	return m_entries.begin();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_iterator vector_map<K, V, T, A >::begin() const
{
	return m_entries.begin();
}

template<typename K, typename V, typename T, typename A>
void vector_map<K, V, T, A >::clear()
{
	m_entries.resize(0);
}

template<typename K, typename V, typename T, typename A>
void vector_map<K, V, T, A >::clearAndFreeMemory()
{
	stl::free_container(m_entries);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::size_type vector_map<K, V, T, A >::capacity() const
{
	return m_entries.capacity();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::size_type vector_map<K, V, T, A >::count(const key_type& key) const
{
	return size_type(std::binary_search(m_entries.begin(), m_entries.end(), value_type(key, mapped_type()), static_cast<key_compare>(*this)));
}

template<typename K, typename V, typename T, typename A>
bool vector_map<K, V, T, A >::empty() const
{
	return m_entries.empty();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::end()
{
	return m_entries.end();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_iterator vector_map<K, V, T, A >::end() const
{
	return m_entries.end();
}

template<typename K, typename V, typename T, typename A>
std::pair<typename vector_map<K, V, T, A>::iterator, typename vector_map<K, V, T, A>::iterator> vector_map<K, V, T, A >::equal_range(const key_type& key)
{
	iterator lower = lower_bound(key);
	if (lower != m_entries.end() && key_compare::operator()(key, (*lower).first))
		lower = m_entries.end();
	iterator upper = lower;
	if (upper != m_entries.end())
		++upper;
	return std::make_pair(lower, upper);
}

template<typename K, typename V, typename T, typename A>
std::pair<typename vector_map<K, V, T, A>::const_iterator, typename vector_map<K, V, T, A>::const_iterator> vector_map<K, V, T, A >::equal_range(const key_type& key) const
{
	const_iterator lower = lower_bound(key);
	if (lower != m_entries.end() && key_compare::operator()(key, (*lower).first))
		lower = m_entries.end();
	const_iterator upper = lower;
	if (upper != m_entries.end())
		++upper;
	return std::make_pair(lower, upper);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::erase(iterator where)
{
	return m_entries.erase(where);
}

template<typename K, typename V, typename T, typename A>
template<typename Predicate>
void vector_map<K, V, T, A >::erase_if(const Predicate& predicate)
{
	m_entries.erase(std::remove_if(m_entries.begin(), m_entries.end(), predicate), m_entries.end());
	std::sort(m_entries.begin(), m_entries.end(), FirstLess(static_cast<key_compare>(*this)));
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::erase(iterator first, iterator last)
{
	return m_entries.erase(first, last);
}

template<typename K, typename V, typename T, typename A>
void vector_map<K, V, T, A >::erase(const key_type& key)
{
	iterator where = find(key);

	if (where != m_entries.end())
		m_entries.erase(where);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::find(const key_type& key)
{
	iterator it = lower_bound(key);
	if (it != m_entries.end() && key_compare::operator()(key, (*it).first))
		it = m_entries.end();
	return it;
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_iterator vector_map<K, V, T, A >::find(const key_type& key) const
{
	const_iterator it = lower_bound(key);
	if (it != m_entries.end() && key_compare::operator()(key, (*it).first))
		it = m_entries.end();
	return it;
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::allocator_type vector_map<K, V, T, A >::get_allocator() const
{
	return m_entries.get_allocator();
}

template<typename K, typename V, typename T, typename A>
std::pair<typename vector_map<K, V, T, A>::iterator, bool> vector_map<K, V, T, A >::insert(const value_type& val)
{
	iterator it = lower_bound(val.first);
	bool insertionMade = false;
	if (it == m_entries.end() || key_compare::operator()(val.first, (*it).first))
		it = m_entries.insert(it, val), insertionMade = true;
	return std::make_pair(it, insertionMade);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::insert(iterator where, const value_type& val)
{
	return insert(val);
}

template<typename K, typename V, typename T, typename A>
template<class InputIterator> void vector_map<K, V, T, A >::insert(InputIterator first, InputIterator last)
{
	for (; first != last; ++first)
		insert(*first);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::key_compare vector_map<K, V, T, A >::key_comp() const
{
	return static_cast<key_compare>(*this);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::lower_bound(const key_type& key)
{
	int count = 0;
	count = m_entries.size();
	iterator first = m_entries.begin();
	iterator last = m_entries.end();
	for (; 0 < count; )
	{
		// divide and conquer, find half that contains answer
		int count2 = count / 2;
		iterator mid = first + count2;

		if (key_compare::operator()(mid->first, key))
			first = ++mid, count -= count2 + 1;
		else
			count = count2;
	}
	return first;
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_iterator vector_map<K, V, T, A >::lower_bound(const key_type& key) const
{
	int count = 0;
	count = m_entries.size();
	const_iterator first = m_entries.begin();
	const_iterator last = m_entries.end();
	for (; 0 < count; )
	{
		// divide and conquer, find half that contains answer
		int count2 = count / 2;
		const_iterator mid = first + count2;

		if (key_compare::operator()(mid->first, key))
			first = ++mid, count -= count2 + 1;
		else
			count = count2;
	}
	return first;
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::size_type vector_map<K, V, T, A >::max_size() const
{
	return m_entries.max_size();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::reverse_iterator vector_map<K, V, T, A >::rbegin()
{
	return m_entries.rbegin();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_reverse_iterator vector_map<K, V, T, A >::rbegin() const
{
	return m_entries.rbegin();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::reverse_iterator vector_map<K, V, T, A >::rend()
{
	return m_entries.rend();
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_reverse_iterator vector_map<K, V, T, A >::rend() const
{
	return m_entries.rend();
}

template<typename K, typename V, typename T, typename A>
void vector_map<K, V, T, A >::reserve(size_type count)
{
	m_entries.reserve(count);
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::size_type vector_map<K, V, T, A >::size() const
{
	return m_entries.size();
}

template<typename K, typename V, typename T, typename A>
void vector_map<K, V, T, A >::swap(vector_map& other)
{
	m_entries.swap(other.m_entries);
	std::swap(static_cast<key_compare&>(*this), static_cast<key_compare&>(other));
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::iterator vector_map<K, V, T, A >::upper_bound(const key_type& key)
{
	iterator upper = lower_bound(key);
	if (upper != m_entries.end() && !key_compare::operator()(key, (*upper).first))
		++upper;
	return upper;
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::const_iterator vector_map<K, V, T, A >::upper_bound(const key_type& key) const
{
	iterator upper = lower_bound(key);
	if (upper != m_entries.end() && !key_compare::operator()(key, (*upper).first))
		++upper;
	return upper;
}

template<typename K, typename V, typename T, typename A>
typename vector_map<K, V, T, A>::mapped_type& vector_map<K, V, T, A >::operator[](const key_type& key)
{
	iterator it = find(key);
	if (it == m_entries.end())
		it = insert(value_type(key, mapped_type())).first;
	return (*it).second;
}

#endif //__SORTEDVECTOR_H__