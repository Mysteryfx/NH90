#ifndef _ed_string_h_
#define _ed_string_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include "ed_string.h"

#include <string>

namespace ed {

// ed::sprintf was replaced by ed::string_printf in Util/Strings.h

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
typedef std::basic_string<char, std::char_traits<char>, ed::allocator<char> > string;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, ed::allocator<wchar_t> > wstring;
#else
typedef std::string string;
typedef std::wstring wstring;
#endif

} // namespace ed

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#if _MSC_VER < 1700
namespace std {

template<>
	class hash<ed::string>
		: public unary_function<ed::string, size_t>
	{	// hash functor
public:
	typedef ed::string _Kty;

	size_t operator()(const _Kty& _Keyval) const{
		// hash _Keyval to size_t value by pseudorandomizing transform
		size_t _Val = 2166136261U;
		size_t _First = 0;
		size_t _Last = _Keyval.size();
		size_t _Stride = 1 + _Last / 10;

		for(; _First < _Last; _First += _Stride)
			_Val = 16777619U * _Val ^ (size_t)_Keyval[_First];
		return (_Val);
	}
};

template<>
	class hash<ed::wstring>
		: public unary_function<ed::wstring, size_t>
	{	// hash functor
public:
	typedef ed::wstring _Kty;

	size_t operator()(const _Kty& _Keyval) const{
		// hash _Keyval to size_t value by pseudorandomizing transform
		size_t _Val = 2166136261U;
		size_t _First = 0;
		size_t _Last = _Keyval.size();
		size_t _Stride = 1 + _Last / 10;

		for(; _First < _Last; _First += _Stride)
			_Val = 16777619U * _Val ^ (size_t)_Keyval[_First];
		return (_Val);
	}
};
}
#endif

inline std::ostream& operator<<(std::ostream& os, const ed::string &str) 
{
	os << str.c_str();
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const ed::wstring &str) 
{
	os << str.c_str();
	return os;
}

#endif

#endif /* _ed_string_h_ */
