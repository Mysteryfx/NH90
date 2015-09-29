#ifndef _ROUGH_EARTH_2D_H_
#define _ROUGH_EARTH_2D_H_

#include "Models/RoughEarth.h"

#include "Utils/ArrayRange.h"
#include <ed/vector.h>

namespace Radio
{

class RoughEarth2D: public RoughEarth
{
public:
	RoughEarth2D(const ReflectingSurface & reflectingSurfaceIn, Flags flags = FLAG_ALL);
	template<size_t size>
	RoughEarth2D(const Vec2 (&reliefIn)[size], const ReflectingSurface & reflectingSurfaceIn, Flags flags = FLAG_ALL) :
		RoughEarth(reflectingSurfaceIn, flags), relief_(reliefIn, reliefIn + size)
	{

	}
	template<class Iterator>
	RoughEarth2D(Iterator begin, Iterator end, const ReflectingSurface & reflectingSurfaceIn, Flags flags = FLAG_ALL) :
		RoughEarth(reflectingSurfaceIn, flags), relief_(begin, end)
	{
	}
protected:
	virtual ArrayRange<const Vec2*> getRelief_(const Vec3 & from, const Vec3 & to) const;
	virtual ArrayRange<const Vec2*> getReliefWithMedian_(const Vec3 & from, const Vec3 & to, const Vec2 & median) const;
	virtual void getReflectionArea_(const Scene & scene, ReflectingArea & reflectingArea) const;
	virtual Height getHeight(const Vec3 & point) const;
	typedef ed::vector<Vec2> Relief;
	Relief						relief_;
	mutable ed::vector<Vec2>	processedRelief_;
};

}

#endif _ROUGH_EARTH_2D_H_