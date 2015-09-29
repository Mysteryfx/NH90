#pragma once

#include "MathExport.h"
#include "ClipVolume.h"

class ClipVolumeCircle : public ClipVolume
{
public:
	MATH_API virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);
	
	float cx, cy, cz;
	float r;
	bool test3D;

	MATH_API ClipVolumeCircle(float _cx, float _cy, float _cz, float _r, bool _test3D);

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);

	MATH_API virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};
