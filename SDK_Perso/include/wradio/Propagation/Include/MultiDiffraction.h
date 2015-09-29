#ifndef _MULTI_DIFFRACTION_H_
#define _MULTI_DIFFRACTION_H_

#include "Definitions.h"

#include "Utils/ArrayRange.h"

namespace Radio
{

struct MultiObstacle
{
	Path2D					path2D;
	ArrayRange<const Vec2*> range;
};

Gain burlingtonDb(const MultiObstacle & multiObstacle, Lambda lambda, bool cylinder);

}

#endif _MULTI_DIFFRACTION_H_