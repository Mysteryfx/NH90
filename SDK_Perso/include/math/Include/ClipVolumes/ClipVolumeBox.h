#pragma once

#include "MathExport.h"
#include "ClipVolume.h"

#include "GraphMath/Box.h"

class MATH_API ClipVolumeBox : public ClipVolume
{
public:
	ClipVolumeBox()
	{
		x=y=z=X=Y=Z=0.f;
	}
	ClipVolumeBox(const Box& box)
	{
		x=box.min.x; y=box.min.y; z=box.min.z;
		X=box.max.x; Y=box.max.y; Z=box.max.z;
	}

	ClipVolumeBox(float bx, float by, float bz, float bX, float bY, float bZ)
	{
		x=bx; y=by; z=bz;
		X=bX; Y=bY; Z=bZ;
	}

	void Set(const Box& box)
	{
		x=box.min.x; y=box.min.y; z=box.min.z;
		X=box.max.x; Y=box.max.y; Z=box.max.z;
	}

	void Set(float bx, float by, float bz, float bX, float bY, float bZ)
	{
		x=bx; y=by; z=bz;
		X=bX; Y=bY; Z=bZ;
	}

	virtual void Prepare(float landx, float landz, float landX, float landZ, int TreePerCell) {}

	virtual IntersectionType IntersectSquare(float bx, float bz, float bX, float bZ)
	{
		int inside=0, contains=0;

		if((bX<x) || (bx>X)) return itOutside;
		if((bZ<z) || (bz>Z)) return itOutside;
		
		if((bx>=x) && (bX<=X)) contains++;
		else if((bx<=x) && (bX>=X)) inside++;

		if((bz>=z) && (bZ<=Z)) contains++;
		else if((bz<=z) && (bZ>=Z)) inside++;

		if(contains == 2) return itContains;
		else if(inside == 2) return itInside;
		else return itIntersects;
	}

	virtual IntersectionType IntersectBox(float bx, float by, float bz, float bX, float bY, float bZ)
	{
		int inside=0, contains=0;

		if((bX<x) || (bx>X)) return itOutside;
		if((bZ<z) || (bz>Z)) return itOutside;
		if((bY<y) || (by>Y)) return itOutside;

		if((bx>=x) && (bX<=X)) contains++;
		else if((bx<=x) && (bX>=X)) inside++;

		if((by>=y) && (bY<=Y)) contains++;
		//else if((by<=y) && (bY>=Y)) inside++;

		if((bz>=z) && (bZ<=Z)) contains++;
		else if((bz<=z) && (bZ>=Z)) inside++;

		if(contains == 3) return itContains;
		else if(inside == 2) return itInside;
		else return itIntersects;
	}
};
