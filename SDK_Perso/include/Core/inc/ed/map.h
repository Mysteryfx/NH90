#ifndef _ed_map_h_
#define _ed_map_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <map>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

template<typename Key, typename T, typename Compare=std::less<Key>,
#ifdef USE_NEDMALLOC_STL_ALLOCATOR
    typename Alloc=ed::allocator<std::pair<const Key, T> > >
#else
    typename Alloc=std::allocator<std::pair<const Key, T> > >
#endif
class map : public std::map<Key, T, Compare, Alloc>
{
    typedef std::map<Key, T, Compare, Alloc> base;
public:
    // construction
    explicit map (const Compare& comp = Compare())
        : base(comp)
    {}

    template <class InputIterator>
    map ( InputIterator first, InputIterator last, const Compare& comp = Compare())
        : base(first, last, comp)
    {}

    // copying
    map(const map&x)
        : base(x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "map(const map&x)");
#endif
	}

	map(map&& other):base(std::move(other))
	{}

    const map& operator = (const map& x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "const map& operator = (const map& x)");
#endif
        base::operator=(x);
        return *this;
    }

    map& operator=( map&& other ){
		base::operator=(std::move(other));
        return *this;
	}
};

} // namespace ed

#endif /* _ed_map_h_ */
