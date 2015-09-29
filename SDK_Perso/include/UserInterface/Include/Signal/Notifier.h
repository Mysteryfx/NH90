/**
 * @file Signal/Notifier.h
 * @author Dmitry S. Baikov
 */
#ifndef SIGNAL_NOTIFIER_H
#define SIGNAL_NOTIFIER_H


#include "./Evoker.h"
#include "./Proxies.h"

namespace Signal {


template<class _Event>
class Notifier;


/**
 * Base class for all notifiers
 */
class NotifierBase : protected Evoker {
	typedef Evoker Base;
public:
	/**
	 * Attach class Logic { void proc(); };
	 */
	template<class _Logic>
	void attach( _Logic* logic, void (_Logic::*proc)())
	{
		typedef _VoidProxy<_Logic> Proxy;
		insert( Proxy::caller, new Proxy(logic, proc), Proxy::destroyer );
	}

	/**
	 * Attach class Logic { void proc(Param); };
	 */
	template<class _Logic, class T>
	void attach( _Logic* logic, void (_Logic::*proc)(T), const T& param )
	{
		typedef _ParamProxy<_Logic, T> Proxy;
		insert( Proxy::caller, new Proxy(logic, proc, param), Proxy::destroyer );
	}

	/**
	 * Detach all class Logic { void ?(?); };
	 */
	template<class _Logic>
	void detach(_Logic *key)
	{
		remove( 0, &key, sizeof(key) );
	}

	/**
	 * Detach class Logic { void proc(); };
	 */
	template<class _Logic>
	void detach(_Logic *logic, void (_Logic::*proc)())
	{
		_VoidProxy<_Logic> key( logic, proc );
		remove( 0, &key, sizeof(key) );
	}

	/**
	 * Detach class Logic { void proc(Arg); };
	 */
	template<class _Logic, class _Arg>
	void detach(_Logic *logic_ptr, void (_Logic::*proc)(_Arg))
	{
		_ArgProxy<_Logic, _Arg> key( logic_ptr, proc );
		remove( 0, &key, sizeof(key) );
	}

	/**
	 * Detach all subscribers.
	 */
	using Base::clear;

protected:
	/**
	 * Raise a signal with argument e
	 */
	void notify( void* e ) { evoke( e ); }

}; // class NotifierBase


/**
 * Dumb notifier.
 */
template<>
class Notifier<void> : public NotifierBase {
	typedef NotifierBase Base;
public:
	/**
	 * Notify all listeners
	 */
	void notify() { Base::notify(0); }

}; // class Notifier<void>


/**
 * Common notifier
 */
template<class _Event>
class Notifier : public NotifierBase {
	typedef NotifierBase Base;
public:
	using Base::attach;
	using Base::detach;

	/**
	 * Attach class Logic { void proc(_Event); };
	 */
	template<class _Logic>
	void attach( _Logic* logic, void (_Logic::*proc)(_Event) )
	{
		typedef _ArgProxy<_Logic, _Event> Proxy;
		insert( Proxy::caller, new Proxy(logic, proc), Proxy::destroyer );
	}

	/*
	 * detach<_Logic, _Arg>() inherited from Base
	 */
	// ----

	/**
	 * Notify the event to all listeners
	 */
	void notify(_Event event) { Base::notify( &event ); }

}; // class Notifier<_Event>


}; // namespace Signal


#endif /* SIGNAL_NOTIFIER_H */
