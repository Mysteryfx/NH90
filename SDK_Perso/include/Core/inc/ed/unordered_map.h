#ifndef _ed_unordered_map_h_
#define _ed_unordered_map_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <unordered_map>
#include <ed/string.h>

#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
#include <assert.h>
#endif

namespace ed {

template<typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>,
#ifdef USE_NEDMALLOC_STL_ALLOCATOR
    typename Alloc=ed::allocator<std::pair<const Key, T> > >
#else
    typename Alloc=std::allocator<std::pair<const Key, T> > >
#endif
class unordered_map : public std::unordered_map<Key, T, Hash, Pred, Alloc>
{
    typedef std::unordered_map<Key, T, Hash, Pred, Alloc> base;
public:
    typedef typename base::hasher hasher;
    typedef typename base::key_equal key_equal;

    // construction
    explicit unordered_map(typename base::size_type n = 3,
                           const hasher& hf = hasher(),
                           const key_equal& eql = key_equal())
        : base(n, hf, eql)
    {}

    template <class InputIterator>
    unordered_map(InputIterator f, InputIterator l,
                  typename base::size_type n = 3,
                  const hasher& hf = hasher(),
                  const key_equal& eql = key_equal())
        : base(f, l, n, hf, eql)
    {}

    // copying
    unordered_map(const unordered_map &x)
        : base(x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "unordered_map(const unordered_map&x)");
#endif
	}

	unordered_map(unordered_map&& other):base(std::move(other))
	{}

    const unordered_map& operator = (const unordered_map &x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "const unordered_map& operator = (const unordered_map& x)");
#endif

        base::operator=(x);
        return *this;
    }

    unordered_map& operator=( unordered_map&& other ){
		base::operator=(std::move(other));
        return *this;
	}

};

template<typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>,
#ifdef USE_NEDMALLOC_STL_ALLOCATOR
    typename Alloc=ed::allocator<std::pair<const Key, T> > >
#else
    typename Alloc=std::allocator<std::pair<const Key, T> > >
#endif
class unordered_multimap : public std::unordered_multimap<Key, T, Hash, Pred, Alloc>
{
    typedef std::unordered_multimap<Key, T, Hash, Pred, Alloc> base;
public:
    typedef typename base::hasher hasher;
    typedef typename base::key_equal key_equal;

    // construction
    explicit unordered_multimap(typename base::size_type n = 3,
                           const hasher& hf = hasher(),
                           const key_equal& eql = key_equal())
        : base(n, hf, eql)
    {}

    template <class InputIterator>
    unordered_multimap(InputIterator f, InputIterator l,
                  typename base::size_type n = 3,
                  const hasher& hf = hasher(),
                  const key_equal& eql = key_equal())
        : base(f, l, n, hf, eql)
    {}

    // copying
    unordered_multimap(const unordered_multimap &x)
        : base(x)
    {
#ifdef EDSTL_ENABLE_ASSERT_ON_BIG_COPY
		assert(x.size() < 100 && "unordered_multimap(const unordered_multimap&x)");
#endif
	}

	unordered_multimap(unordered_multimap &&x)
        : base(std::move(x))
    {
	}

    unordered_multimap& operator = (unordered_multimap &&x)
    {
        base::operator=(std::move(x));
        return *this;
    }

};

} // namespace ed

#endif /* _ed_unordered_map_h_ */
