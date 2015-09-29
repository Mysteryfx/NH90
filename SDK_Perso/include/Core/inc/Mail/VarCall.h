// variadic template argument caller
// (c)2002-2012 Dmitry S. Baikov <dsb@eagle.ru>
#ifndef _ed_Mail_VarCall_h_
#define _ed_Mail_VarCall_h_


#include "./VarArgs.h"


namespace Mail {


template<int N> struct _VarDispatch;


template<class P_, class M_, class A_>
void call(P_ object, M_ method, const A_& args)
{
	/*return*/ _VarDispatch< A_::Length >::call(object, method, args);
}

template<class F_, class A_>
void call(F_ func, const A_& args)
{
	/*return*/ _VarDispatch< A_::Length >::call(func, args);
}


#define _VAR_GEN_CALLER(LENGTH, ARGS) \
template<> struct _VarDispatch<LENGTH> { \
	template<class P_, class M_, class A_> static \
	void call(P_ object, M_ method, const A_& args) \
	{ /*return*/ (object->*method)ARGS; } \
	\
	template<class F_, class A_> static \
	void call(F_ func, const A_& args) \
	{ /*return*/ (*func)ARGS; } \
};

_VAR_GEN_CALLER(0, ())
_VAR_GEN_CALLER(1, (args.value \
					))
_VAR_GEN_CALLER(2, (args.value, \
					args.next.value \
					))
_VAR_GEN_CALLER(3, (args.value, \
					args.next.value, \
					args.next.next.value \
					))
_VAR_GEN_CALLER(4, (args.value, \
					args.next.value, \
					args.next.next.value, \
					args.next.next.next.value \
					))
_VAR_GEN_CALLER(5, (args.value, \
					args.next.value, \
					args.next.next.value, \
					args.next.next.next.value, \
					args.next.next.next.next.value \
					))
_VAR_GEN_CALLER(6, (args.value, \
					args.next.value, \
					args.next.next.value, \
					args.next.next.next.value, \
					args.next.next.next.next.value, \
					args.next.next.next.next.next.value \
					))
_VAR_GEN_CALLER(7, (args.value, \
					args.next.value, \
					args.next.next.value, \
					args.next.next.next.value, \
					args.next.next.next.next.value, \
					args.next.next.next.next.next.value, \
					args.next.next.next.next.next.next.value \
					))
_VAR_GEN_CALLER(8, (args.value, \
					args.next.value, \
					args.next.next.value, \
					args.next.next.next.value, \
					args.next.next.next.next.value, \
					args.next.next.next.next.next.value, \
					args.next.next.next.next.next.next.value, \
					args.next.next.next.next.next.next.next.value \
					))
_VAR_GEN_CALLER(9, (args.value, \
					args.next.value, \
					args.next.next.value, \
					args.next.next.next.value, \
					args.next.next.next.next.value, \
					args.next.next.next.next.next.value, \
					args.next.next.next.next.next.next.value, \
					args.next.next.next.next.next.next.next.value, \
					args.next.next.next.next.next.next.next.next.value \
					))

#undef _VAR_GEN_CALLER

} // Mail namespace

#endif /* _ed_VarCall_h_ */
