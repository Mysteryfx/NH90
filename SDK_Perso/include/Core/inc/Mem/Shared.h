// intrusive reference counting
// (c)2001-2012 Dmitry S. Baikov <dsb@eagle.ru>
#ifndef _ED_Mem_Shared_h_
#define _ED_Mem_Shared_h_


#include "tbb/atomic.h"

namespace Mem {

/**
 * Base class for reference counted objects.
 * @note Destructor is non-virtual to support light-weight objects
 */
class SharedLite {
	tbb::atomic<int> m_refs;
public:
	SharedLite() { m_refs = 0; }
	~SharedLite() {}

    friend void shared_ptr_acquire(SharedLite* ptr);
    friend bool shared_ptr_release(SharedLite* ptr);
};

inline void shared_ptr_acquire(SharedLite* ptr) { if(ptr) ++(ptr->m_refs); }
inline bool shared_ptr_release(SharedLite* ptr) { return (ptr && --(ptr->m_refs)==0); }

/**
 * Base class for heavy-weight shared objects.
 */
class Shared : public SharedLite {
public:
	Shared() {}
	virtual ~Shared() {}
};


/**
 * Smart pointer using reference counting.
 * Expects two external functions to work for T*:
 *   inline void shared_ptr_acquire(T* ptr);
 *   inline bool shared_ptr_release(T* ptr);
 */
template<class T>
class Ptr {
private:
	T* ptr;
	void acquire(T* p) { shared_ptr_acquire(p); }
	void release(T* p) { if( shared_ptr_release(p) ) delete p; }

	class Tester { void operator delete(void*) {} };

public:
		Ptr() : ptr(0) {}
		Ptr(T* p) : ptr(p) { acquire(ptr); }
		Ptr(const Ptr<T>& r) : ptr(r.ptr) { acquire(ptr); }
		~Ptr() { release(ptr); }

	Ptr& operator = (const Ptr<T>& r) { acquire(r.ptr); release(ptr); ptr = r.ptr; return *this; }
	Ptr& operator = (T* p) { acquire(p); release(ptr); ptr = p; return *this; }

	T* operator -> () const { return ptr; }

	T* get() const { return ptr; }

	bool valid()const{return ptr!=NULL;}

	T& operator*() { return *ptr; }
	const T& operator*() const { return *ptr; }

	//Comparison operators
	template<class _U> inline bool operator == (const Ptr<_U>& p){return get() == p.get();}
	template<class _U> inline bool operator != (const Ptr<_U>& p){return get() != p.get();}

	template<class _U> inline friend bool operator == (const Ptr& p,const _U *r){return p.get() == r;}
	template<class _U> inline friend bool operator == (const _U *r,const Ptr& p){return p.get() == r;}
                              
	template<class _U> inline friend bool operator != (const Ptr& p,const _U *r){return p.get() != r;}
	template<class _U> inline friend bool operator != (const _U *r,const Ptr& p){return p.get() != r;}

	//This operators work when comparing with NULL
	inline friend bool operator == (const Ptr& p,const T *r){return p.get() == r;}
	inline friend bool operator == (const T *r,const Ptr& p){return p.get() == r;}

	inline friend bool operator != (const Ptr& p,const T *r){return p.get() != r;}
	inline friend bool operator != (const T *r,const Ptr& p){return p.get() != r;}

	// if(p) and if(!p)
	operator Tester*() const { return reinterpret_cast<Tester*>(ptr); }

}; // class Ptr

// should not be used, ~SharedLite() is not virtual.
template<> class Ptr<SharedLite> {};

// forbidden, just because
template<> class Ptr<Shared> {};

} // Mem namespace

#endif /* _ED_Mem_Shared_h_ */
