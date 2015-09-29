// implements C++11-style signal interface
#ifndef _ed_signal_h_
#define _ed_signal_h_

#include "../_config.h"

namespace ed {

class signal {
public:
        ED_CORE_EXPORT signal(bool initial_state);
        ED_CORE_EXPORT ~signal();

    /**
     * Set signal to alerted state.
     */
    ED_CORE_EXPORT void set();

    /**
     * Reset signal to unalerted state.
     */
    ED_CORE_EXPORT void reset();

    /**
     * Wait until the signal becomes alerted.
     * @note the signal stays alerted
     */
    ED_CORE_EXPORT void wait();

    /**
     * Check if the signal is alerted.
     * @retval true the signal is in the alerted state
     * @retval false the signal is not alerted
     */
    ED_CORE_EXPORT bool try_wait();

    struct Impl { void *opaque[1]; };
private:
    Impl _impl;
};

} // namespace ed

#endif /* _ed_signal_h_ */
