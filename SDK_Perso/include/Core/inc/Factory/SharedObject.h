#ifndef __ED_SHARED_OBJECT_H
#define __ED_SHARED_OBJECT_H

namespace Common {

class ISharedObject
{
	int m_refs;
	virtual void FinalRelease() = 0;

public:
	ISharedObject() : m_refs(0) {}
	virtual ~ISharedObject() {}

	void AddRef() { ++m_refs; }
	void Release() { if(--m_refs == 0) FinalRelease(); }
};


template<class T>
class Unknown : public T
{
	virtual void FinalRelease() { delete this; }
};

template<class T>
class FakeUnknown : public T
{
	virtual void FinalRelease() {/*NOTHING!*/}
};

/**
 * CComPtr<T>
 */

template<class T>
class SharedPtr {
private:
	T* ptr;

	// the policy:
	void acquire(T* p) { if (p) p->AddRef(); }
	void release(T* p) { if (p) p->Release(); }

public:
		SharedPtr() : ptr(0) {}
		SharedPtr(T* p) : ptr(p) { acquire(ptr); }
		SharedPtr(const SharedPtr<T>& r) : ptr(r.ptr) { acquire(ptr); }
		~SharedPtr() { release(ptr); }

	SharedPtr& operator = (const SharedPtr<T>& r) { acquire(r.ptr); release(ptr); ptr = r.ptr; return *this; }
	SharedPtr& operator = (T* p) { acquire(p); release(ptr); ptr = p; return *this; }

	T* operator -> () const { return ptr; }

	T* get() const { return ptr; }

	T& operator*() { return *ptr; }
	const T& operator*() const { return *ptr; }

	// CComPtr compatibility
	operator T* () const { return ptr; }
	//T** operator & () { return &ptr; }
	bool operator!() const { return ptr == 0; }
	bool operator < (T* p) const { return ptr < p; }
	bool operator == (T* p) const { return ptr == p; }
	bool operator != (T* p) const { return ptr != p; }

}; // class Ptr
typedef SharedPtr<ISharedObject> ISharedPtr;

} // namespace Common

#endif /* __ED_SHARED_OBJECT_H */
