#ifndef __PtrKeeper__
#define __PtrKeeper__

namespace EagleFM { namespace Elec {

template<class T>
class PtrKeeper
{
private:
	T* ptr;

public:
	PtrKeeper() : ptr(nullptr) {}
	PtrKeeper(T* p) : ptr(p) {}
	PtrKeeper(const PtrKeeper<T>& r) : ptr(r.ptr) {}

	PtrKeeper& operator = (const PtrKeeper<T>& r) {ptr = r.ptr; return *this; }
	PtrKeeper& operator = (T* p) { ptr = p; return *this; }

	T* operator -> () const { return ptr; }

	T* get() const { return ptr; }

	bool valid() const {return ptr != nullptr;}

	T& operator*() { return *ptr; }
	const T& operator*() const { return *ptr; }

	// Comparison operators
	template<class _U> inline bool operator == (const PtrKeeper<_U>& p){return get() == p.get();}
	template<class _U> inline bool operator != (const PtrKeeper<_U>& p){return get() != p.get();}

	template<class _U> inline friend bool operator == (const PtrKeeper& p,const _U *r){return p.get() == r;}
	template<class _U> inline friend bool operator == (const _U *r,const PtrKeeper& p){return p.get() == r;}
                              
	template<class _U> inline friend bool operator != (const PtrKeeper& p,const _U *r){return p.get() != r;}
	template<class _U> inline friend bool operator != (const _U *r,const PtrKeeper& p){return p.get() != r;}

	// This operators work when comparing with NULL
	inline friend bool operator == (const PtrKeeper& p,const T *r){return p.get() == r;}
	inline friend bool operator == (const T *r,const PtrKeeper& p){return p.get() == r;}

	inline friend bool operator != (const PtrKeeper& p,const T *r){return p.get() != r;}
	inline friend bool operator != (const T *r,const PtrKeeper& p){return p.get() != r;}
};

} }

#endif // __PtrKeeper__
