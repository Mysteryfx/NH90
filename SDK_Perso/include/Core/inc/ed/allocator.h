#ifndef _ed_allocator_h_
#define _ed_allocator_h_

#include <limits>
#include <utility>
#include "nedmalloc.h"
#pragma comment(lib, "nedmalloc.lib")

#if defined(ED_ALLOCATOR_DEBUG) && ED_ALLOCATOR_DEBUG == 2
#include "AllocMonitor.h"
#endif

namespace ed {

// STL-compatible allocator
template <typename T>
class allocator_base {
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef T           value_type;

    pointer address (reference value) {
        return &value;
    }

	const_pointer address (const_reference value) const {
        return &value;
    }

    size_type max_size() const {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    void construct(pointer p, const_reference value) {
        new ((void*)p) T(value);
    }

#ifndef _WINDOWS
    // Because we are compiling with -std=c++0x on gcc it uses variadic templates in STL 
    // implementation. So our custom allocator should support variadic templates.
    // MSVC 2010 does not support variadic templates.
    template <typename... Args> void construct(pointer p, Args&&... args) {
        new ((void*)p) T(std::forward<Args>(args)...);
    }
#endif
    
    void destroy(pointer p) {
        p->~T();
    }

    // override THIS part to change memory management
    pointer allocate(size_type num, const void* hint = 0) {
#if defined(ED_ALLOCATOR_DEBUG) && ED_ALLOCATOR_DEBUG == 2
		auto n = num * sizeof(T);
    	void *p = nedmalloc(n + sizeof(intptr_t));
		incSTLAllocatorCounter(ED_MODULE_NAME, num * sizeof(T), *(intptr_t*)p);
        return reinterpret_cast<pointer>((int8_t*)p + sizeof(intptr_t));
#else
    	return static_cast<pointer>(nedmalloc(num * sizeof(T)));
#endif
    }

    void deallocate(pointer p, size_type num){
#if defined(ED_ALLOCATOR_DEBUG) && ED_ALLOCATOR_DEBUG == 2
		if(p == nullptr){
			return;
		}

		decSTLAllocatorCounter(num * sizeof(T), *(intptr_t*)((int8_t*)p - sizeof(intptr_t)));
        nedfree((int8_t*)p - sizeof(intptr_t));
#else
        nedfree((void*)p);//, num*sizeof(T));
#endif
    }
}; // class allocator_base

template <typename T>
class allocator : public allocator_base<T> {
public:
    allocator() {}
    allocator(const allocator&) {}
    template <typename U>
    allocator(const allocator<U>&) {}

    template <typename U>
    struct rebind {
        typedef allocator<U> other;
    };

    typename allocator_base<T>::const_pointer address (typename allocator_base<T>::const_reference value) const {
        return &value;
    }
}; // class allocator<T>

template <typename T>
class allocator<const T> : public allocator_base<const T> {
public:
    allocator() {}
    allocator(const allocator&) {}
    template <typename U>
    allocator(const allocator<U>&) {}

    template <typename U>
    struct rebind {
        typedef allocator<U> other;
    };
}; // class allocator<const T>

template<>
class allocator<void>{
    public:
    typedef void*        pointer;
    typedef const void*    const_pointer;
    typedef void        value_type;
    template <class U>
    struct rebind{
        typedef allocator<U> other;
    };
}; // class allocator<void>

template <class T1, class T2> inline
bool operator == (const allocator<T1>&, const allocator<T2>) {
    return true;
}

template <class T1, class T2> inline
bool operator != (const allocator<T1>&, const allocator<T2>) {
    return false;
}

} // namespace ed

#endif /* _ed_allocator_h_ */
