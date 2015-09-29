#ifndef _ED_Math_roundDec_h_
#define _ED_Math_roundDec_h_

#include "./_config.h"
#include "Math/Vector.h"

namespace Math
{
	// Rounding up to the required decimal
	inline double ED_MATH_EXTERN roundDec(double val, int binary_digits = 20) 
	{
		return ldexp(floor(ldexp(val, binary_digits) + 0.5), -binary_digits);
	}

	inline Vec3d ED_MATH_EXTERN roundDec(const Vec3d& V, int binary_digits = 20) 
	{
		return Vec3d(roundDec(V.x, binary_digits), roundDec(V.y, binary_digits), roundDec(V.z, binary_digits));
	}
}


#endif // _ED_Math_roundDec_h_
