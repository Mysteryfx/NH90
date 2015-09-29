#ifndef _ed_lockfree_stack_h_
#define _ed_lockfree_stack_h_

#include "./lockfree_utils.h"

namespace ed {

/**
 * Multiple writers - multiple readers lock-free stack
 * (C)2010, 2013 Dmitry S. Baikov <dsbaikov@gmail.com> <dsb@eagle.ru>
 * C++11 version
 */
class lockfree_stack {
public:
    lockfree_stack()
    {
        _top.store(nullptr, std::memory_order_relaxed);
    }

    ~lockfree_stack()
    {
        assert (empty());
    }

    void concurrent_push(lockfree_node* node);
    lockfree_node* concurrent_pop();

    bool empty()
    {
        return _top.load() == nullptr;
    }

private:
    std::atomic<lockfree_node*> _top;
    lockfree_node* hazard_pointer() { return reinterpret_cast<lockfree_node*>(~uintptr_t(0)); }

    lockfree_stat _push_tries;
    lockfree_stat _push_yields;
    lockfree_stat _push_calls;
    lockfree_stat _pop_tries;
    lockfree_stat _pop_yields;
    lockfree_stat _pop_calls;
    lockfree_counter _size;
};

inline
void lockfree_stack::concurrent_push(lockfree_node* value)
{
    assert (value != nullptr);
    lockfree_node* cur_top = _top.load(std::memory_order_consume/*we don't need the contents*/);
    while (1)
    {
        ED_LOCKFREE_STAT(_push_tries);
        // top could be marked as dirty in concurrent pop,
        // so we busy-loop until we see a clean top
        for (int i = 1; cur_top == hazard_pointer(); ++i)
        {
            lockfree_throttle(i, _push_yields);
            cur_top = _top.load(std::memory_order_consume);
        }

        value->chain.store(cur_top, std::memory_order_relaxed);

        // A thread that will pop the node with ed::memory_order_acquire will see all our writes.
        if (_top.compare_exchange_weak(cur_top, value, std::memory_order_release, std::memory_order_consume))
            break;
    }
    ED_LOCKFREE_STAT(_push_calls);
    ++_size;
}

/*
 pop is tricky because we do not own the top item until we actually poped it
 and we can't just update top with 'chain', because another thread could
 pop this very item and change it's 'chain' field.
 that's why we mark top with the special 'hazard' pointer
 to mark our ownership of the top.
 */
inline
lockfree_node* lockfree_stack::concurrent_pop()
{
    lockfree_node* value = _top.load(std::memory_order_acquire/*we need the contents of the node*/);
    while(1)
    {
        ED_LOCKFREE_STAT(_pop_tries);

        for (int i = 1; value == hazard_pointer(); ++i)
        {
            lockfree_throttle(i, _pop_yields);
            value = _top.load(std::memory_order_acquire);
        }

        if (value == nullptr)
            return nullptr;

		// try to lock the top
        if (_top.compare_exchange_weak(value, hazard_pointer(), std::memory_order_release, std::memory_order_acquire))
            break; // we locked the top
    }

    // we can safely use the contents of 'value' because we got it using std::memory_order_acquire.
    lockfree_node* new_top = value->chain.load(std::memory_order_relaxed);
    // Update and unlock the top.
    _top.store(new_top, std::memory_order_release);

    ED_LOCKFREE_STAT(_pop_calls);
    
    --_size;
    return value;
}


template<class T>
class lockfree_stack_of : public lockfree_stack {
public:
    void concurrent_push(T* value) { lockfree_stack::concurrent_push(value); }
    T* concurrent_pop() { return static_cast<T*>(lockfree_stack::concurrent_pop()); }
};

} // namespace ed

#endif /* _ed_lockfree_stack_h_ */
