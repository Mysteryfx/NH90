/**
 * EasyGL library v0.1
 * (C)opyright 2002 by Dmitry S. Baikov
 *
 * @file _math.h
 * @author Dmitry S. Baikov
 */
#ifndef _ED_Math_math_h_
#define _ED_Math_math_h_


#include "./_config.h"


#include <math.h>

namespace Math {
	
#define _ED_MATH_FUNC1( Name ) \
template<class T> inline T Name(T arg) { return T(::Name(arg)); } \
template<> inline float Name(float arg) { return ::Name##f(arg); }

_ED_MATH_FUNC1( sin )
_ED_MATH_FUNC1( asin )

_ED_MATH_FUNC1( cos )
_ED_MATH_FUNC1( acos )

_ED_MATH_FUNC1( atan )

_ED_MATH_FUNC1( sqrt )

#undef _ED_MATH_FUNC1

template <class T>
inline T toRad(T grad)
{return grad * T(M_PI / 180);}

template <class T>
inline T toGrad(T rad)
{return rad * T(180 / M_PI);}

} // namespace Math

#endif /* _ED_Math_math_h_ */
