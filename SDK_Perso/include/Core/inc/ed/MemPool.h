#ifndef _ed_MemPool_h_
#define _ed_MemPool_h_

#ifdef USE_NEDMALLOC_POOL
#include <nedmalloc.h>
#else
#include <stdlib.h>
#endif

namespace ed {

#ifdef USE_NEDMALLOC_POOL
class MemPool {
    size_t _csize;
    nedpool* _pool;
public:
    MemPool(size_t csize, size_t count)
        : _csize(csize)
    {
        _pool = nedcreatepool(csize * count, 0);
    }

    ~MemPool()
    {
        neddestroypool(_pool);
    }

	void* alloc()
    {
        return nedpmalloc(_pool, _csize);
    }

	void free(void* p)
    {
        nedpfree(_pool, p);
    }
};
#else
class MemPool {
    size_t _csize;
public:
    MemPool(size_t csize, size_t count)
        : _csize(csize){}

    ~MemPool(){}

	void* alloc()
    {
        return malloc(_csize);
    }

	void free(void* p)
    {
        ::free(p);
    }
};
#endif

template<class T>
class MemPoolOf : public MemPool {
public:
		MemPoolOf(unsigned count) : MemPool(sizeof(T), count) {}
};


#define MEM_DECL_CLASS_POOL(name) \
	private: static ed::MemPoolOf<name> m_pool; \
	public: void* operator new(size_t) { return m_pool.alloc(); } \
	public: void operator delete(void* p) { m_pool.free(p); }

#define MEM_IMPL_CLASS_POOL(name, size) \
	ed::MemPoolOf<name> name::m_pool(size);

} // namespace ed

#endif /* _ed_MemPool_h_ */
