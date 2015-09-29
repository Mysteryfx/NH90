// Implements core.math simplifications and includes.
//

#ifndef __Math_Defs_h_
#define __Math_Defs_h_

#include "Math/_math.h"
#include "Math/Math.h"

namespace cockpit {

typedef Math::Vec3f Vec3f;
typedef Math::Vec3d Vec3d;
typedef Math::Matrix3f Matrix3f;
typedef Math::Matrix3d Matrix3d;
typedef Math::Rot3f Rot3f;
typedef Math::Rot3d Rot3d;

#define min_max(a, b, c) (__max((a), (__min((b), (c)))))

template <typename T> T sign(T value)
{
	if (value > 0)
		return 1;

	if (value < 0)
		return -1;

	return 0;
}

#define X_axis_f Vec3f(1.0f, 0.0, 0.0)
#define Y_axis_f Vec3f(0.0, 1.0f, 0.0)
#define Z_axis_f Vec3f(0.0, 0.0, 1.0f)

#define X_axis_d Vec3d(1.0, 0.0, 0.0)
#define Y_axis_d Vec3d(0.0, 1.0, 0.0)
#define Z_axis_d Vec3d(0.0, 0.0, 1.0)

}

#endif // __Math_Defs_h_
