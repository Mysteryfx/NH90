#ifndef _ed_lockfree_gather_h_
#define _ed_lockfree_gather_h_

#include "./lockfree_utils.h"


namespace ed {

/**
 * Multiple writers - Single reader lock-free queue
 * (C)2010,2013 Dmitry S. Baikov <dsbaikov@gmail.com> <dsb@eagle.ru>
 * The algorithm was developed in 2010.
 * C++11 version written in 2013
 */
class lockfree_gather {
public:
    lockfree_gather()
        : _read_head(&_read_head)
    {
        //_read_head.chain.store(&_dummy, std::memory_order_relaxed);
        _write_tail.store(&_read_head, std::memory_order_relaxed);
    }

    ~lockfree_gather()
    {
        assert (empty());
    }

    void concurrent_push(lockfree_node* value);
    lockfree_node* serial_pop();

    bool empty()
    {
        return _read_head.chain.load(std::memory_order_relaxed) == &_read_head
            && _write_tail.load(std::memory_order_relaxed) == &_read_head;
    }

private:
    lockfree_node _read_head;
    std::atomic<lockfree_node*> _write_tail;

    lockfree_node* hazard_pointer() { return reinterpret_cast<lockfree_node*>(~uintptr_t(0)); }

    lockfree_stat _push_tries;
    lockfree_stat _push_yields;
    lockfree_stat _push_calls;
    lockfree_stat _pop_tries;
    lockfree_stat _pop_yields;
    lockfree_stat _pop_calls;
    lockfree_counter _size;
}; // class lock_free_gather



inline
void lockfree_gather::concurrent_push(lockfree_node* value)
{
    assert (value != nullptr);

	// link value to the _dummy
    // nobody sees this yet, so the order is relaxed
    value->chain.store(&_read_head, std::memory_order_relaxed);
	
    /* --- concurrent secton A --- */
    // now we need to lock the write tail
    lockfree_node* old_tail = _write_tail.load(std::memory_order_acquire); // acquire-release pair AR-1
    while (1)
    {
        ED_LOCKFREE_STAT(_push_tries);
        for (int i = 1; old_tail == hazard_pointer(); ++i)
        {
            lockfree_throttle(i, _push_yields);
            old_tail = _write_tail.load(std::memory_order_acquire); // AR-1
        }
		
        if (_write_tail.compare_exchange_weak(old_tail, hazard_pointer(), std::memory_order_release, std::memory_order_acquire))
            break; // we locked the tail
    }

    assert (old_tail != hazard_pointer());
    assert (old_tail != nullptr);
	
    // Make sure the node at old_tail is the last one: either it points to the read_head or is the read_head itself
    // The second part of assert may fail if concurrent pop is between sections B and C,
	// then the second part comes into play.
    assert (old_tail == &_read_head || old_tail->chain.load(std::memory_order_relaxed) == &_read_head);
	
    /* --- concurrent secton B --- */
	// Link value to the old tail.
    // this line breaks while-loop in pop:B
    old_tail->chain.store(value, std::memory_order_release); // acquire-release pair AR-2
    
    /* --- concurrent secton C --- */
	/// Update the write_tail.
    // this line unlocks concurrent pushes
    _write_tail.store(value, std::memory_order_release); // acquire-release pair AR-1

    ED_LOCKFREE_STAT(_push_calls);
    ++_size;
}

inline
lockfree_node* lockfree_gather::serial_pop()
{
	lockfree_node* value;

    /* --- concurrent secton A --- */
    value = _read_head.chain.load(std::memory_order_acquire); // acquire-release pair AR-2

    if (value == &_read_head)
        return nullptr; // empty

    /* --- concurrent secton B --- */
    // this loop runs only when the value is the last entry and
    // a concurrent_push has not completed the section B yet
    for (int i = 1; value->chain.load(std::memory_order_acquire) == &_read_head; ++i)
    {
        ED_LOCKFREE_STAT(_pop_tries);
        lockfree_node* tmp = value;
        // this CAS will fail if a concurrent_push completed section A
        if (_write_tail.compare_exchange_weak(tmp, &_read_head, std::memory_order_release, std::memory_order_acquire))
            break;
        lockfree_throttle(i, _pop_yields);
    }

    /* --- concurrent secton C --- */
    lockfree_node* new_tail = value->chain.load(std::memory_order_acquire);
    // this CAS will fail only if the queue was empty (i.e. &head == tail)
    // and another thread pushed a value (in push:B),
	// but in such case write is not needed anyway.
    lockfree_node* tmp = value;
	_read_head.chain.compare_exchange_strong(tmp, new_tail, std::memory_order_release, std::memory_order_relaxed); // acquire-release pair AR-1

    ED_LOCKFREE_STAT(_pop_calls);
    --_size;
    return value;	
}


template<class T>
class lockfree_gather_of : public lockfree_gather {
public:
    void concurrent_push(T* value) { lockfree_gather::concurrent_push(value); }
    T* serial_pop() { return static_cast<T*>(lockfree_gather::serial_pop()); }
};

} // namespace ed

#endif /* _ed_lockfree_gather_h_ */
