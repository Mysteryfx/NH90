#ifndef _ed_deque_h_
#define _ed_deque_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <deque>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
template<typename T, typename Alloc=ed::allocator<T> >
#else
template<typename T, typename Alloc=std::allocator<T> >
#endif
class deque : public std::deque<T, Alloc> {
    typedef std::deque<T, Alloc> base;
public:
    // construction
    deque()
        : base()
    {}

    explicit deque(typename base::size_type n, const T& value=T())
        : base(n, value)
    {}

    template <class InputIterator>
    deque (InputIterator first, InputIterator last)
        : base(first, last)
    {}

    // copying
    deque(const deque& v) : base(v)   
	{
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(v.size() < 100 && "deque(const deque& v)");
#endif
	}

	deque(deque&& other) : base(std::move(other))
	{}

    const deque& operator = (const deque& v)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(v.size() < 100 && "const deque& operator = (const deque& v)");
#endif
        base::operator=(v);
        return *this;
    }

    deque& operator=( deque&& other ){
		base::operator=(std::move(other));
        return *this;
	}
};

} // namespace ed

#endif /* _ed_deque_h_ */
