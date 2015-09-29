// variadic template argument list
// (c)2002-2012 Dmitry S. Baikov <dsb@eagle.ru>
#ifndef _ed_Mail_VarArgs_h_
#define _ed_Mail_VarArgs_h_


#define VAR_GENERIC_INPUT
#define VAR_INPUT_OPERATOR >>

//#define VAR_GENERIC_OUTPUT
//#define VAR_OUTPUT_OPERATOR <<


namespace Mail {


struct VarNil {
	enum { Length = 0 };
	VarNil() {}
}; // struct VarNil


template<class T>
struct VarLengthOf { enum { Value = 1 }; };


template<>
struct VarLengthOf<VarNil> { enum { Value = 0 }; };


template<class V, class N=VarNil>
struct VarList {
	typedef V value_t;
	typedef N next_t;

	enum { Length = VarLengthOf<V>::Value + N::Length };

	V value;
	N next;

	N& operator = (const V& v) { value = v; return next; }
	N& operator , (const V& v) { value = v; return next; }

}; // struct List


#ifdef VAR_GENERIC_OUTPUT
template<class S_, class V_, class N_> inline
S_& operator VAR_OUTPUT_OPERATOR (S_& s, const VarList<V_, N_>& v)
{
	s VAR_OUTPUT_OPERATOR v.value VAR_OUTPUT_OPERATOR v.next;
	return s;
}

template<class S_> inline
S_& operator VAR_OUTPUT_OPERATOR (S_& s, const VarNil& v)
{
	return s;
}
#endif


#ifdef VAR_GENERIC_INPUT
template<class S_, class V_, class N_> inline
S_& operator VAR_INPUT_OPERATOR (S_& s, VarList<V_, N_>& v)
{
	s VAR_INPUT_OPERATOR v.value VAR_INPUT_OPERATOR v.next;
	return s;
}

template<class S_> inline
S_& operator VAR_INPUT_OPERATOR (S_& s, VarNil& v)
{
	return s;
}
#endif

template<class A1, class A2=VarNil, class A3=VarNil, class A4=VarNil, class A5=VarNil, class A6=VarNil, class A7=VarNil, class A8=VarNil, class A9=VarNil>
struct VarArgs {
	typedef VarList<A1,
				VarList<A2,
					VarList<A3,
						VarList<A4,
							VarList<A5,
								VarList<A6,
									VarList<A7,
										VarList<A8,
											VarList<A9>
										>
									>
								>
							>
						>
					>
				>
			> Type;
}; // struct VarArgs


} // Mail namespace

#endif /* _ed_Mail_VarArgs_h_ */
