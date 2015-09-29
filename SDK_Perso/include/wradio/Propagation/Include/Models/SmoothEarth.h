#ifndef _SMOOTH_EARTH_H_
#define _SMOOTH_EARTH_H_

#include "Environment.h"
#include "Reflection.h"

namespace Radio
{

struct ReflectingArea;

class SmoothEarth: public Environment
{
public:
	SmoothEarth(const ReflectingSurface & reflectingSurfaceIn, Flags flags = FLAG_ALL, bool cylinder = true);
	virtual Gain getGain(const Scene & scene) const;
protected:
	Gain getFreeSpaceGain_(const Scene & scene) const;
	virtual Gain getDiffractionGainDb_(const Scene & scene) const;
	Gain getInterferentionGain_(const Scene & scene) const;
	virtual void getReflectionArea_(const Scene & scene, ReflectingArea & reflectingArea) const;
	const Distance	radius_;
	const Real		Rk_;
	const Flags		flags_;
	const bool		cylinder_;
	ReflectingSurface reflectingSurface;
};

}

#endif _SMOOTH_EARTH_H_
