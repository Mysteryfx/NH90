#ifndef __CoreMath_h__
#define __CoreMath_h__

#include "Math/Math.h"
#include "Math/PID.h"

namespace EagleFM
{
	typedef Math::Vec3d Vec3;
	typedef Math::Matrix3f Matrix3;
	typedef Math::Rot3f Rot3;
	typedef Math::PID PID;
	typedef Math::PID_<float> PID_f;
}

#endif