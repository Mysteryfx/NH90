/**
 * @file Signal/Proxies.h
 * @author Dmitry S. Baikov
 */
#ifndef SIGNAL_PROXIES_H
#define SIGNAL_PROXIES_H


namespace Signal {


/**
 * void T::proc(void) proxy.
 */
template<class T>
struct _VoidProxy {
	T* client;
	void (T::*entry)(void);

	_VoidProxy( T* c, void (T::*e)() ) : client(c), entry(e) {}

	static void caller( void* self, void* signal);
	static void destroyer( void* self );
};

template<class T>
void _VoidProxy<T>::caller(void* client, void* signal)
{
	_VoidProxy<T>* info = static_cast<_VoidProxy<T>*>(client);
	((info->client)->*(info->entry))();
}

template<class T>
void _VoidProxy<T>::destroyer(void* self)
{
	delete static_cast< _VoidProxy<T>* >(self);
}



/**
 * void T::proc(E) proxy.
 */
template<class T, class E>
struct _ArgProxy {
	T* client;	
	void (T::*entry)(E);

		_ArgProxy( T* c, void (T::*e)(E) ) : client(c), entry(e) {}

	static void caller( void* client, void* signal);
	static void destroyer( void* self );
};

template<class T, class E>
void _ArgProxy<T,E>::caller( void* client, void* signal )
{
	_ArgProxy<T,E>* info = static_cast<_ArgProxy<T,E>*>(client);
	((info->client)->*(info->entry))( *static_cast<E*>(signal) );
}

template<class T, class E>
void _ArgProxy<T,E>::destroyer( void* self )
{
	_ArgProxy<T,E>* data = static_cast<_ArgProxy<T,E>*>(self);
	delete data;
}


/**
 * void T::proc(param) proxy.
 */
template<class C, class P>
struct _ParamProxy {
	C* client;	
	void (C::*entry)(P);
	P param;

	_ParamProxy( C* c, void (C::*e)(P), const P& p) : client(c), entry(e), param(p) {}

	static void caller( void* client, void* signal);
	static void destroyer( void* self ) ;
};

template<class C, class P>
void _ParamProxy<C, P>::caller( void* client, void* signal )
{
	_ParamProxy<C, P>* info = static_cast<_ParamProxy<C, P>*>(client);
	((info->client)->*(info->entry))( info->param );
}

template<class C, class P>
void _ParamProxy<C,P>::destroyer( void* self )
{
	_ParamProxy<C,P>* data = static_cast<_ParamProxy<C,P>*>(self);
	delete data;
}


} // namespace Signal


#endif /* SIGNAL_PROXIES_H */
