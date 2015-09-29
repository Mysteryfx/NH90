#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

namespace CoreMeta
{

namespace type_traits_private
{
	// Selector
	template <bool b, class A, class B> struct select {};
	template <class A, class B> struct select<true, A, B> { typedef A value; };
	template <class A, class B> struct select<false, A, B> { typedef B value; };

	// Reference
	template <class T> struct is_reference { enum { value = false }; };
	template <class T> struct is_reference<T&> { enum { value = true }; };

	// Pointer
	template <class T> struct is_pointer			{ enum { value = false }; };
	template <class T> struct is_pointer<T*>		{ enum { value = true }; };
	template <class T> struct is_pointer<T*&>		{ enum { value = true }; };

	// Standard types
	template <class T> struct is_standard { enum { value = false }; };
	template <> struct is_standard<signed char> { enum { value = true }; };
	template <> struct is_standard<short int> { enum { value = true }; };
	template <> struct is_standard<int> { enum { value = true }; };
	template <> struct is_standard<long int> { enum { value = true }; };
	template <> struct is_standard<unsigned char> { enum { value = true }; };
	template <> struct is_standard<unsigned short int> { enum { value = true }; };
	template <> struct is_standard<unsigned int> { enum { value = true }; };
	template <> struct is_standard<unsigned long int> { enum { value = true }; };
	template <> struct is_standard<bool> { enum { value = true }; };
	template <> struct is_standard<char> { enum { value = true }; };
	template <> struct is_standard<float> { enum { value = true }; };
	template <> struct is_standard<double> { enum { value = true }; }; 
	template <> struct is_standard<long double> { enum { value = true }; };
};
template <class U> struct UnConst
{
	typedef U Result;
	enum { isConst = 0 };
};

template <class U> struct UnConst<const U>
{
	typedef U Result;
	enum { isConst = 1 };
};

template <class U> struct UnConst<const U&>
{
	typedef U& Result;
	enum { isConst = 1 };
};

template <class U> struct UnVolatile
{
	typedef U Result;
	enum { isVolatile = 0 };
};

template <class U> struct UnVolatile<volatile U>
{
	typedef U Result;
	enum { isVolatile = 1 };
};

template <class U> struct UnVolatile<volatile U&>
{
	typedef U& Result;
	enum { isVolatile = 1 };
};

template <class U> struct UnReference
{
	typedef U Result;
};

template <class U> struct UnReference<U&>
{
	typedef U Result;
};

template <class U> struct UnReference<const U&>
{
	typedef U Result;
};



template <class T> 
struct TypeTraits
{
public:
	typedef typename UnVolatile<typename UnConst<T>::Result>::Result UnqualifiedType;

	enum { isPointer = type_traits_private::is_pointer<UnqualifiedType>::value };
	enum { isReference = type_traits_private::is_reference<UnqualifiedType>::value };
	enum { isStandard = type_traits_private::is_standard<UnqualifiedType>::value };

	typedef typename type_traits_private::select<isPointer || isReference || isStandard, T, const T&>::value parameter_type;
};

}
#endif