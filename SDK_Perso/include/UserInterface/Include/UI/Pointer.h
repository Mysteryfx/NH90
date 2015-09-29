/**
 * @file UI/Pointer.h
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_POINTER_H
#define GENUI_POINTER_H


#include "./config.h"


namespace UI {


/**
 * Smart UI::Element pointer
 */
template<class T>
class Ptr {
private:
	T* m_ptr;

private:
	bool empty() const { return m_ptr == stub(); }

	void accept( T* ptr )
	{
		m_ptr = ptr ? ptr : stub();
		//if( !empty() ) m_ptr->destroyed().attach( this, &Ptr<T>::release );
	}

	void release()
	{
		//if( !empty() ) m_ptr->destroyed().detach( this );
		m_ptr = stub();
	}

public:
		Ptr() { accept(0); }

		template<class P>
		Ptr( P* ptr ) { accept( dynamic_cast<T*>(ptr) ); }

		Ptr(T* ptr) { accept(ptr); }

		Ptr(const Ptr& p) { accept(p.m_ptr); }

		~Ptr() { release(); }

	Ptr& operator = (T* ptr)
	{
		release();
		accept( ptr );
		return *this;
	}

	T* operator -> () const { return m_ptr; }
	T& operator * () const { return m_ptr; }

	operator bool () const { return !empty(); }

	operator T* () const { return empty() ? 0 : m_ptr; }

	T* ptr() const { return m_ptr; }

	static GENUI_API T* stub();

}; // class Ptr<T>


} // namespace UI

#endif /* GENUI_POINTER_H */
