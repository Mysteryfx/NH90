#ifndef _ed_list_h_
#define _ed_list_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <list>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
template<typename T, typename Alloc=ed::allocator<T> >
#else
template<typename T, typename Alloc=std::allocator<T> >
#endif
class list : public std::list<T, Alloc> {
    typedef std::list<T, Alloc> base;
public:
    // construction
    list()
        : base()
    {}

    explicit list(typename base::size_type n, const T& value=T())
        : base(n, value)
    {}

    template <class InputIterator>
    list (InputIterator first, InputIterator last)
        : base(first, last)
    {}

    // copying
    list(const list& v) : base(v) {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(v.size() < 100 && "list(const list&x)");
#endif
	}

	list(list&& other) : base(std::move(other))
	{}

    const list& operator = (const list& v)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(v.size() < 100 && "const list& operator = (const list& x)");
#endif
        base::operator=(v);
        return *this;
    }
    
    list& operator=( list&& other ){
		base::operator=(std::move(other));
        return *this;
	}
};

} // namespace ed

#endif /* _ed_list_h_ */
