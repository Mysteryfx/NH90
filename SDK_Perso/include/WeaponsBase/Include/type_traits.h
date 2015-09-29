#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

#include <ed/string.h>

template <class T>
struct type_traits
{
	typedef T& reference_type;
	typedef const T  const_param_type;
};

template <>
struct type_traits<ed::string>
{
	typedef string& reference_type;
	typedef const T& const_param_type;
};


#endif 
