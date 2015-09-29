#ifndef _ed_vector_h_
#define _ed_vector_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <vector>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
template<typename T, typename Alloc=ed::allocator<T> >
#else
template<typename T, typename Alloc=std::allocator<T> >
#endif
class vector : public std::vector<T, Alloc> {
    typedef std::vector<T, Alloc> base;
public:
    // construction
    vector()
        : base()
    {}

    explicit vector(typename base::size_type n, const T& value=T())
        : base(n, value)
    {}

    template <class InputIterator>
    vector (InputIterator first, InputIterator last)
        : base(first, last)
    {}

    // copying
    vector(const vector& v) : base(v)   
	{
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(v.size() < 100 && "vector(const vector& v)");
#endif
	}

	vector(vector&& other) : base(std::move(other))
	{}

    const vector& operator = (const vector& v)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(v.size() < 100 && "const vector& operator = (const vector& v)");
#endif
        base::operator=(v);
        return *this;
    }

    vector& operator=( vector&& other ){
		base::operator=(std::move(other));
        return *this;
	}

#ifdef CHECK_VECTOR_RANGE
	reference operator[]( size_type pos ){return at(pos);}
	const_reference operator[]( size_type pos ) const{return at(pos);}
#endif

	/// Removes first appearence of given value by replacing it by the last element.
	bool unstableRemove(const T& value){
		for (typename base::iterator it = this->begin(); it != this->end(); ++it)
		{
			if (*it == value){
				*it = back();
				pop_back();
				return true;
			}
		}
		return false;
	}

	// Shortcut for really long: std::find(vec.begin(), vec.end(), value) != vec.end
	bool contains(const T& value) const
	{
		for (typename base::const_iterator it = this->begin(); it != this->end(); ++it)
		{
			if (*it == value)
				return true;
		}

		return false;
	}
};

} // namespace ed

#endif /* _ed_vector_h_ */
