/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_GEN_HANDLER_H
#define MAIL_GEN_HANDLER_H


#include "./Handler.h"
#include "./Message.h"
#include "./Reader.h"
#include "./VarCall.h"


namespace Mail {


template<class Msg_, class Policy_>
class GenHandler : public Handler {
private:
	Policy_ policy;

public:
		GenHandler(const Policy_& p) : Handler(Msg_::ID, Msg_::Flags), policy(p) {}

	bool handle(const Message&);

}; // class GenHandler<Msg_, Policy_>


template<class Msg_, class Policy_>
bool GenHandler<Msg_, Policy_>::handle(const Message& msg)
{
	typename Msg_::Args args;
	Reader(msg) >> args;
	return policy.handle(args);
}


#if 0
/* ---=== Example Policy ===--- */
template<class O_, class M_>
struct _MemberHandler {
	M_ handler;

	_MemberHandler(M_ m) : handler(m) {}

	template<class A_>
	bool handle(const A_& args)
	{
		O_* obj = args.value;
		if( obj==0 ) return false;
		Mail::call(obj, handler, args.next);
		return true;
	}

};


template<class O_, class M_>
_MemberHandler<O_, M_> memberHandler(M_ method)
{
	return _MemberHandler<O_, M_>(method);
}
#endif


// static function handler
template<class M_, class F_>
class _StaticHandler : public Mail::Handler {
	F_ func;
public:
		_StaticHandler(F_ f, const char* name) : Mail::Handler(M_::ID, M_::Flags, name), func(f) {}

	virtual bool handle(const Mail::Message& msg) {
		typename M_::Args args;
		Mail::Reader r(msg);
		r >> args;
		Mail::call(func, args);
		return true;
	}
}; // class _StaticHandler


// generator
template<class M_>
struct GenStaticHandler {

template<class F_> inline
Mail::HandlerPtr operator()(F_ func, const char* name=0)
{
	return Mail::HandlerPtr( new _StaticHandler<M_, F_>(func, name) );
}

};

// PtrHandler

template<class M_, class P_, class F_>
class PtrHandler : public Mail::Handler {
	P_ ptr;
	F_ func;
public:
	PtrHandler(P_ p, F_ f, const char* name)
		: Handler(M_::ID, M_::Flags, name), ptr(p), func(f) {}
	bool handle(const Mail::Message& msg);
};

template<class M_, class P_, class F_>
bool PtrHandler<M_, P_, F_>::handle(const Mail::Message& msg)
{
	typename M_::Args args;
	Mail::Reader reader(msg);
	reader >> args;
	Mail::call(ptr, func, args);
	return true;
}


template<class M_>
struct GenPtrHandler {

template<class P_, class F_>
Mail::HandlerPtr operator()(P_ ptr, F_ func, const char* name)
{ return Mail::HandlerPtr(new PtrHandler<M_, P_, F_>(ptr, func, name)); }

};

// MsgHandler

template<class M_, class P_, class F_>
class MsgHandler : public Mail::Handler {
	P_ ptr;
	F_ func;
public:
	MsgHandler(P_ p, F_ f, const char* name)
		: Handler(M_::ID, M_::Flags, name), ptr(p), func(f) {}
	bool handle(const Mail::Message& msg);
};

template<class M_, class P_, class F_>
bool MsgHandler<M_, P_, F_>::handle(const Mail::Message& msg)
{
	(ptr->*func)(msg);
	return true;
}


template<class M_>
struct GenMsgHandler {

template<class P_, class F_>
Mail::HandlerPtr operator()(P_ ptr, F_ func, const char* name)
{ return Mail::HandlerPtr(new MsgHandler<M_, P_, F_>(ptr, func, name)); }

};

} // Mail namespace

#endif /* MAIL_GEN_HANDLER_H */
