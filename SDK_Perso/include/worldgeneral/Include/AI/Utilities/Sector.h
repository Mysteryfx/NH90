#ifndef __Sector__
#define __Sector__

//Operations with sectors

#include "WorldGeneral.h"

class WORLDGENERAL_API Sector
{
public:
	enum IntersectionResult
	{
		NO_INTERSECTION,
		INSIDE,
		CONTAINS,
		ONE_INTERSECTION,
		TWO_INTERSECTIONS,
		EQUAL,
	};
	enum CENTER_AND_WIDTH_ENUM
	{
		CENTER_AND_WIDTH
	};
	enum START_END_ENUM
	{
		FROM_TO
	};
	static Sector Circle;
	Sector();
	Sector(float center_, float halfWidth_, CENTER_AND_WIDTH_ENUM);
	Sector(float center_, float halfWidth_, START_END_ENUM);
	void				setCenterHalfWidth(float center_, float halfWidth_);
	void				setStartEnd(float start_, float end_);	
	bool operator		== (const Sector & sector) const;
	inline float		getWidth() const { return 2 * halfWidth; }
	inline float		getLeft() const { return center - halfWidth; }
	inline float		getRight() const { return center + halfWidth; }
	bool				isInside(float angle) const;
	static IntersectionResult	intersect(const Sector & sector1, const Sector & sector2, Sector * result = nullptr);	
public:
	float center;
	float halfWidth;
};

#endif