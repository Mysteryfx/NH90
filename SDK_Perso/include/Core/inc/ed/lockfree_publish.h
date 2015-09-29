#ifndef _ed_lockfree_publish_h_
#define _ed_lockfree_publish_h_

#include "./lockfree_utils.h"

namespace ed {

/**
 * LockFree 1 writer, N readers publishing point
 * reads are not wait-free
 * writes are wait-free
 * WARNING: T should be a POD data type.
 * (C)2013 Dmitry S. Baikov <dsb@eagle.ru>
 * Based on the idea from jackaudio.org
 *
 * Difference from RCU: writer is wait-free, readers may spin-lock
 */
template<class T>
class lockfree_publish {
public:
    typedef uintptr_t tag_type;

        lockfree_publish()
        {
            _beginTag.store(0);
            _endTag.store(0);
        }

        ~lockfree_publish()
        {
        }

    tag_type serial_write(const T& post)
    {
        _beginTag.fetch_add(1, std::memory_order_release);
        _data = post;
        tag_type tag = _endTag.fetch_add(1, std::memory_order_release);
        ED_LOCKFREE_STAT(_nwrites);
        return tag;
    }

    tag_type concurrent_read(T& result)
    {
        tag_type tag;
        int i = 0;
        do {
            lockfree_throttle(i, _nyields);
            ++i;
            ED_LOCKFREE_STAT(_ntries);
            // reading the _endTag with acquire makes sure we see the proper _data
            tag = _endTag.load(std::memory_order_acquire);
            result = _data;
            // we read the _beginTag with consume because we don't need anything else
        } while (_beginTag.load(std::memory_order_consume) != tag);
        ED_LOCKFREE_STAT(_nreads);
        return tag;
    }

    bool concurrent_read_new(T& result, tag_type& tag_memory)
    {
        tag_type new_tag = concurrent_read(result);
        if (new_tag == tag_memory)
            return false;
        tag_memory = new_tag;
        return true;
    }

    void get_stats(uint64_t& nreads, uint64_t& ntries)
    {
        nreads = _nreads;
        ntries = _ntries;
    }

private:
    std::atomic<tag_type> _beginTag;
    T _data;
    std::atomic<tag_type> _endTag;

    lockfree_stat _nwrites;
    lockfree_stat _ntries;
    lockfree_stat _nyields;
    lockfree_stat _nreads;

};

} // namespace ed

#endif /* _ed_lockfree_publish_h_ */
