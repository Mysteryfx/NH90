#ifndef _REFLECTION_H_
#define _REFLECTION_H_

#include "Definitions.h"

namespace Radio
{

struct Material
{
	Real		epsilon;//relative dielectric constant
	Real		sigma;	//electric conductance, Simens/m
};

struct ReflectingSurface
{
	Material	material;
	bool		smooth; //smooth plane
	Height		dh;		//mean sqr of height deviation, m^2
};

struct ReflectingArea
{
	ReflectingSurface	surface;
	Vec2				point;
	Vec2				normal;
};

struct ReflectionGain
{
	Gain directGain;
	Gain reflectionGain;
	Angle dPhase;
};

void getInterferentionGain(	const Vec2 & from, const Vec2 & to,
							const Signal & signal,
							const ReflectingArea & reflectingArea,
							ReflectionGain & reflectionGain); //direct + reflected beams

//ComplexGain getReflectionGain(	const Scene & scene,
//								const ReflectingArea & reflectingArea); //only reflected beam

}

#endif _REFLECTION_H_