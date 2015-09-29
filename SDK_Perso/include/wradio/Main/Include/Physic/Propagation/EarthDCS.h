#ifndef _EARTH_DCS_H_
#define _EARTH_DCS_H_

#include "Models/RoughEarth.h"

using namespace Radio;

class EarthDCS: public RoughEarth
{
public:
	EarthDCS(const ReflectingSurface & reflectingSurfaceIn, Flags flags = FLAG_ALL);
protected:
	virtual ArrayRange<const Vec2*> getRelief_(const Vec3 & from, const Vec3 & to) const;
	virtual ArrayRange<const Vec2*> getReliefWithMedian_(const Vec3 & from, const Vec3 & to, const Vec2 & median) const;
	virtual Height getHeight(const Vec3 & point) const;
};

#endif _EARTH_DCS_H_