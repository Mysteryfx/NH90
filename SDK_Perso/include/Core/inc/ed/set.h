#ifndef _ed_set_h_
#define _ed_set_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <set>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

template<typename Key, typename Compare=std::less<Key>,
#ifdef USE_NEDMALLOC_STL_ALLOCATOR
    typename Alloc=ed::allocator<Key> >
#else
    typename Alloc=std::allocator<Key> >
#endif
class set : public std::set<Key, Compare, Alloc>
{
    typedef std::set<Key, Compare, Alloc> base;
public:
    // construction
    explicit set (const Compare& comp = Compare())
        : base(comp)
    {}

    template <class InputIterator>
    set ( InputIterator first, InputIterator last, const Compare& comp = Compare())
        : base(first, last, comp)
    {}

    // copying
    set(const set&x)
        : base(x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "set(const set&x)");
#endif
	}

	set(set&& other) : base(std::move(other))
	{}

    const set& operator = (const set& x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "const set& operator = (const set& x)");
#endif
        base::operator=(x);
        return *this;
    }

    set& operator=(set&& other ){
		base::operator=(std::move(other));
        return *this;
	}
};

} // namespace ed

#endif /* _ed_set_h_ */
