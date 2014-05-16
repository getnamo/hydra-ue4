/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_STRING_HPP
#define SIXENSE_UTILS_STRING_HPP

SIXENSE_UTILS_EXPORT void* sixense_utils_alloc(size_t);
SIXENSE_UTILS_EXPORT void sixense_utils_free(void*); 

#include <string>
#include <cstddef>

template<class MY_TYPE>
class sixense_utils_allocator
{
public:
	typedef size_t              size_type;
	typedef ptrdiff_t   difference_type;
	typedef MY_TYPE*    pointer;
	typedef const MY_TYPE*      const_pointer;
	typedef MY_TYPE&    reference;
	typedef const MY_TYPE&      const_reference;
	typedef MY_TYPE             value_type;

	sixense_utils_allocator () {}
	template <class U> sixense_utils_allocator(const sixense_utils_allocator<U>&) {}
	template <class U> struct rebind { typedef sixense_utils_allocator<U> other; };

	pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0)
	{
		return reinterpret_cast<pointer>(sixense_utils_alloc(n * sizeof(MY_TYPE)));
	}
	void construct(pointer p, const_reference val)
	{
		::new(p) MY_TYPE(val);
	}
	void destroy(pointer p)
	{
		p->~MY_TYPE();
	}
	void deallocate(pointer p, size_type n)
	{
		sixense_utils_free(p);
	}
	size_type max_size() const throw()
	{
		return ~size_type(0);
	}
};

template<class MY_TYPE>
bool operator==(const sixense_utils_allocator<MY_TYPE>& left, const sixense_utils_allocator<MY_TYPE>& right) throw()
{
	return true;
}

template<class MY_TYPE>
bool operator!=(const sixense_utils_allocator<MY_TYPE>& left, const sixense_utils_allocator<MY_TYPE>& right) throw()
{
	return false;
}

namespace sixenseUtils {

	typedef std::basic_string< char, std::char_traits<char>, sixense_utils_allocator<char> > sixense_utils_string;

}


#endif

