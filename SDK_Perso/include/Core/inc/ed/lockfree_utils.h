#ifndef _ed_lockfree_utils_h_
#define _ed_lockfree_utils_h_

#include <atomic>
#include <thread>


namespace ed {

struct lockfree_node;

struct lockfree_node {
    std::atomic<lockfree_node*> chain;

    lockfree_node() {}
    lockfree_node(lockfree_node* n)
    {
        chain.store(n, std::memory_order_relaxed);
    }
};

struct lockfree_counter {
    std::atomic<uint64_t> value;

    lockfree_counter()
    {
        value.store(0, std::memory_order_relaxed);
    }

    uint64_t operator = (const uint64_t v)
    {
        value.store(v, std::memory_order_relaxed);
        return v;
    }

    uint64_t reset()
    {
        return value.exchange(0, std::memory_order_relaxed);
    }

    uint64_t operator++()
    {
        return value.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    uint64_t operator--()
    {
        return value.fetch_add(-1, std::memory_order_relaxed) - 1;
    }

    operator uint64_t() const
    {
        return value.load(std::memory_order_relaxed);
    }
};

#ifdef DEBUG
struct lockfree_stat : public lockfree_counter
{
};

#define ED_LOCKFREE_STAT(x) ++x

#else

struct lockfree_stat {
    void operator++() {}
    void operator--() {}
    operator uint64_t() { return 0; }
};
#define ED_LOCKFREE_STAT(x)
#endif

inline void lockfree_throttle(int& i, lockfree_stat& stat)
{
    if (i > 16)
    {
        i = 0;
        ++stat;
        std::this_thread::yield();
    }
}

} // namespace ed

#endif /* _ed_lockfree_utils_h_ */
