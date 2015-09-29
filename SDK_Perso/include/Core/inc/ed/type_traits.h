#ifndef ED_CORE_TYPE_TRAITS_H
#define ED_CORE_TYPE_TRAITS_H

#include <type_traits>

namespace ed {

template<typename T, bool IsElemental = std::is_scalar<T>::value> struct TypeTraits{};

template<typename T> struct TypeTraits<T, true>{
	typedef const T const_ref_t;
};
	
template<typename T> struct TypeTraits<T, false>{
	typedef const T& const_ref_t;
};

}

#endif
