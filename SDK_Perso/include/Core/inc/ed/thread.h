// C++11 std::thread-compatible WinAPI-implementation
// Written by Dmitry S. Baikov <dsb@eagle.ru>
#ifndef _ed_thread_h_
#define _ed_thread_h_

#include "../_config.h"
#include <stdint.h>
#include <functional>

namespace ed {

class thread {
public:
    typedef uintptr_t native_handle_type;
    class id {
        friend class thread;
        uintptr_t _value;

        id(uintptr_t v) : _value(v) {}
    public:
        id() : _value(0) {}
        bool operator==(const id& cmp) { return _value == cmp._value; }
        bool operator!=(const id& cmp) { return _value != cmp._value; }
        bool operator<(const id& cmp) { return _value < cmp._value; }
        bool operator<=(const id& cmp) { return _value <= cmp._value; }
        bool operator>(const id& cmp) { return _value > cmp._value; }
        bool operator>=(const id& cmp) { return _value >= cmp._value; }
    };

    ED_CORE_EXTERN thread();
    ED_CORE_EXTERN thread(std::function<void()> fn);

    // move
    thread(thread&& moved)
    {
        _hThread = moved._hThread;
        moved.detach();
    }

    thread& operator=(thread&& moved)
    {
        _hThread = moved._hThread;
        moved.detach();
        return *this;
    }

    ~thread() { join(); }


    ED_CORE_EXTERN bool joinable();
    ED_CORE_EXTERN void join();
    ED_CORE_EXTERN id get_id() const { return id(_hThread); }
    native_handle_type native_handle() { return _hThread; }

    //ED_CORE_EXTERN static unsigned hardware_concurrency();

private:
    native_handle_type _hThread;
    static unsigned int __stdcall _thread_func(void* argptr);
    ED_CORE_EXTERN void detach();

    struct FuncPtr {
        std::function<void()> func;
        FuncPtr(std::function<void()> f) : func(f) {}
    };

    thread(const thread&){}
    void operator=(const thread&) {}
};


namespace this_thread {

ED_CORE_EXTERN void yield();

} // namespace this_thread

} // namespace ed

#endif /* _ed_thread_h_ */
