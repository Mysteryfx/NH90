// implements C++11-alike semaphore interface
#ifndef _ed_semaphore_h_
#define _ed_semaphore_h_

#include "../_config.h"
#ifdef DEBUG
#include <atomic>
#endif

namespace ed {

class semaphore {
public:
        ED_CORE_EXPORT semaphore(int initial_count);
        ED_CORE_EXPORT ~semaphore();

    ED_CORE_EXPORT void post();
    ED_CORE_EXPORT void wait();
    ED_CORE_EXPORT bool try_wait();

    struct Impl { void *opaque[1]; };
private:
    Impl _impl;
#ifdef DEBUG
    std::atomic<int> _value;
#endif
};

} // namespace ed

#endif /* _ed_semaphore_h_ */
