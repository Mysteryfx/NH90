// C++11 std::mutex-compatible WinAPI-implementation
#ifndef _ed_mutex_h_
#define _ed_mutex_h_

#include "../_config.h"

namespace ed {

class mutex {
public:
    ED_CORE_EXTERN mutex();
    ED_CORE_EXTERN ~mutex();

    ED_CORE_EXTERN void lock();
    ED_CORE_EXTERN bool try_lock();
    ED_CORE_EXTERN void unlock();

    struct Impl {
        void* opaque[6]; // WinAPI CRITICAL_SECTION
    };
private:
    Impl _impl;

    mutex(const mutex&) {}
    void operator=(const mutex&) {}
};

typedef mutex recursive_mutex;

template<class T>
class lock_guard {
    T& _lock;
    lock_guard(const lock_guard&);
    void operator=(const lock_guard&);
public:
    lock_guard(T& lock) : _lock(lock) { _lock.lock(); }
    ~lock_guard() { _lock.unlock(); }
};

} // namespace std

#endif /* _ed_mutex_h_ */
