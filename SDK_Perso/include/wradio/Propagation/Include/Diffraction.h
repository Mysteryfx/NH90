#ifndef _DIFFRACTION_H_
#define _DIFFRACTION_H_

#include "Definitions.h"

namespace Radio
{

struct RawObstacle : public Path2D
{
	Distance	d1;			//horizontal distance from transmitter to the obstacle, m
	Height		ho;			//Height of the obstacle, m
	bool		cylinder;	//the obstacle is a cylinder
	Distance	R;			//cylinder radius, m
};

struct Obstacle
{
	Distance	d1;			//horizontal distance from transmitter to the obstacle, m
	Distance	d2;			//horizontal distance from the obstacle to the receiver, m
	Height		h;			//obstacle height over transmitter-receiver line, m
	bool		cylinder;	//is cylinder
	Distance	R;			//cylinder radius, m
};

void convertObstacle(const RawObstacle & raw, Obstacle & result);

Gain getDiffractionGainDb(const Obstacle & obstacle, Lambda lambda);

inline Gain getDiffractionGain(const Obstacle & obstacle, Lambda lambda)
{
	return pow(10.0, getDiffractionGainDb(obstacle, lambda) / 20.0);
}

}

#endif _DIFFRACTION_H_