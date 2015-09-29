#ifndef _ROUGH_EARTH_H_
#define _ROUGH_EARTH_H_

#include "Models/SmoothEarth.h"

#include "Utils/ArrayRange.h"

namespace Radio
{

class RoughEarth: public SmoothEarth
{
public:
	RoughEarth(const ReflectingSurface & reflectingSurfaceIn, Flags flags = FLAG_ALL, bool cylinder = true);
protected:
	virtual Gain getDiffractionGainDb_(const Scene & scene) const;
	virtual ArrayRange<const Vec2*> getRelief_(const Vec3 & from, const Vec3 & to) const = 0;
	virtual ArrayRange<const Vec2*> getReliefWithMedian_(const Vec3 & from, const Vec3 & to, const Vec2 & median) const = 0;
	virtual void getReflectionArea_(const Scene & scene, ReflectingArea & reflectingArea) const;
	virtual Height getHeight(const Vec3 & point) const = 0;
};

}

#endif _ROUGH_EARTH_H_