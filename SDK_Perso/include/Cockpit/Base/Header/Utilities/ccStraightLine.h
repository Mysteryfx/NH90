#ifndef __straight_line_h__
#define __straight_line_h__

#pragma once

#include "Vector.h"
#include "CockpitBase.h"

namespace cockpit
{
class COCKPITBASE_API StraightLine
{
public:	
	float A, B, C;

	StraightLine(){}
	StraightLine(float, float, float);

	StraightLine(const Vector3&,const Vector3&);
	StraightLine(const Vector3&,const Vector3&,const Vector3&);

	inline StraightLine operator - () const;
	inline const StraightLine&	operator ()(const Vector3&,const  Vector3&);
	inline const StraightLine&	operator ()(const Vector3&,const  Vector3&,const  Vector3&);
	inline const StraightLine&	operator ()(float, float, float);

	inline Vector3 intersection(const StraightLine&) const;
	inline double point_side(const Vector3&) const;

	// shortest distance from line to the point
	inline double distance(const Vector3&) const;
};
#include "ccStraightLine.inl"

}

#endif // __straight_line_h__
