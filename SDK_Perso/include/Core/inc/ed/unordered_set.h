#ifndef _ed_unordered_set_h_
#define _ed_unordered_set_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <unordered_set>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

template<typename T, typename Hash = std::hash<T>, typename Pred = std::equal_to<T>,
#ifdef USE_NEDMALLOC_STL_ALLOCATOR
    typename Alloc=ed::allocator<T> >
#else
    typename Alloc=std::allocator<T> >
#endif
class unordered_set : public std::unordered_set<T, Hash, Pred, Alloc>
{
    typedef std::unordered_set<T, Hash, Pred, Alloc> base;
public:
	typedef typename base::hasher hasher;
    typedef typename base::key_equal key_equal;

    // construction
    explicit unordered_set(typename base::size_type n = 3,
                           const hasher& hf = hasher(),
                           const key_equal& eql = key_equal())
        : base(n, hf, eql)
    {}

    template <class InputIterator>
    unordered_set(InputIterator f, InputIterator l,
                  typename base::size_type n = 3,
                  const hasher& hf = hasher(),
                  const key_equal& eql = key_equal())
        : base(f, l, n, hf, eql)
    {}

    // copying
    unordered_set(const unordered_set &x)
        : base(x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "unordered_set(const unordered_set&x)");
#endif
	}

	unordered_set(unordered_set&& other) : base(std::move(other))
	{}

    const unordered_set& operator = (const unordered_set &x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "const unordered_set& operator = (const unordered_set& x)");
#endif

        base::operator=(x);
        return *this;
    }

	unordered_set& operator=(unordered_set&& other ){
		base::operator=(std::move(other));
        return *this;
	}

};

} // namespace ed

#endif /* _ed_unordered_set_h_ */
