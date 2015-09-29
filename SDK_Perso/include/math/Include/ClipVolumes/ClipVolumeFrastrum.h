#pragma once

#include "MathExport.h"
#include "ClipVolume.h"
#include "Matrix33.h"

class MATH_API ClipVolumeFrastrum : public ClipVolume
{
protected:
	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell)
	{
		frastrum.GetFrastrumBox(x, z, X, Z);
	}

public:
	oldmath::Matrix33 frastrum;

	ClipVolumeFrastrum(const oldmath::Matrix33 &_frastrum, float yMin, float yMax) :
	  frastrum(_frastrum)
	{
		y = yMin;
		Y = yMax;
	}

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ)
	{
		if((bX<x) || (bx>X)) return itOutside;
		if((bZ<z) || (bz>Z)) return itOutside;
		return frastrum.IntersectBox(bx, bz, bX, bZ);
	}
	
	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ)
	{
		if((bX<x) || (bx>X)) return itOutside;
		if((bZ<z) || (bz>Z)) return itOutside;
		if((bY<y) || (by>Y)) return itOutside;
		IntersectionType it = frastrum.IntersectBox(bx, bz, bX, bZ);
		if((it==itContains) && ((by<y) || (bY>Y))) it = itIntersects;
		else if((it==itInside) && ((by>y) || (bY<Y))) it = itIntersects;
		return it;
	}
};

