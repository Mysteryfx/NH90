#ifndef _ARRAY_RANGE_H_
#define _ARRAY_RANGE_H_

#include <ed/vector.h>

template<typename Iterator> 
struct ArrayRange
{
	ArrayRange() : begin(NULL), end(NULL) {;}
	ArrayRange(Iterator beginIn, Iterator endIn) : begin(beginIn), end(endIn) {;}
	inline size_t size() const { return end - begin; }
	inline size_t empty() const { return size() < 1; }
	Iterator begin;
	Iterator end;
};

template<typename T, typename A>
ArrayRange<T*> arrayRange(typename ed::vector<T, A> & v)
{
	return ArrayRange<T*>(&*v.begin(), &*v.begin() + (v.size()));
}

template<typename T, typename A>
ArrayRange<T*> arrayRange(typename ed::vector<T, A>::iterator from,
						  typename ed::vector<T, A>::iterator to)
{
	return ArrayRange<T*>(&*from, &*to);
}

template<typename T, typename A>
ArrayRange<const T*> arrayRange(const typename ed::vector<T, A> & v)
{
	return ArrayRange<const T*>(&*v.begin(), &*v.begin() + (v.size()));
}

template<typename T, typename A>
ArrayRange<const T*> arrayRange(typename ed::vector<T, A>::const_iterator from,
								typename ed::vector<T, A>::const_iterator to)
{
	return ArrayRange<const T*>(&*from, &*to);
}

template<typename T, size_t size>
ArrayRange<T*> arrayRange(T (&t)[size])
{
	return ArrayRange<T*>(t, t + size);
}

template<typename T>
ArrayRange<T*> arrayRange(T & t)
{
	return ArrayRange<T*>(&t, &t + 1);
}

#endif