#ifndef _ED_CORE_MEM_H_
#define _ED_CORE_MEM_H_

#if defined(ED_ALLOCATOR_DEBUG) && ED_ALLOCATOR_DEBUG == 2

#include <new>

#include "ed/AllocMonitor.h"

inline void* ed_custom_alloc(size_t n)
{
	void *p = malloc(n + sizeof(intptr_t));
	auto mn = _msize(p);
	ed::incNEWAllocatorCounter(ED_MODULE_NAME, mn, *(intptr_t*)((int8_t*)p + mn - sizeof(intptr_t)));
	return p;
}

inline void ed_custom_free(void *p)
{
	if(p == nullptr) return; // as _msize doesn't work with null pointers
	auto n = _msize(p);
	ed::decNEWAllocatorCounter(n, *(intptr_t*)((int8_t*)p + n - sizeof(intptr_t)));
	free(p);
}

inline void* operator new(size_t n)
{
	return ed_custom_alloc(n);
}
inline void* operator new(size_t n, const std::nothrow_t&)
{
	return ed_custom_alloc(n);
}
inline void* operator new[](size_t n)
{
	return ed_custom_alloc(n);
}
inline void* operator new[](size_t n, const std::nothrow_t&)
{
	return ed_custom_alloc(n);
}
inline void operator delete(void *p)
{
	ed_custom_free(p);
}
inline void operator delete(void* p, const std::nothrow_t&)
{
	ed_custom_free(p);
}
inline void operator delete[](void* p)
{
	ed_custom_free(p);
}
inline void operator delete[](void* p, const std::nothrow_t&)
{
	ed_custom_free(p);
}
#endif
#endif
