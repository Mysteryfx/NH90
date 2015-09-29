#pragma once

#include "MathExport.h"
#include "cPosition.h"
#include "ClipVolume.h"

class MATH_API ClipVolumeSegment : public ClipVolume
{
private:
	cPoint A, B;
	float len;
protected:
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell);
public:
	ClipVolumeSegment(const cPoint& a, const cPoint& b);
	
	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ);
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ);
};
