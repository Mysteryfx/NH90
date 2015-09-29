#ifndef __WorldPoint__
#define __WorldPoint__

#include "cLinear.h"

struct RefPoint
{
	RefPoint() : callsign(0), type(0) {;}
	RefPoint(const cPoint & point_, unsigned int name_, unsigned int type_) : point(point_), callsign(name_), type(type_) {;}
	unsigned int	callsign;
	unsigned int	type;
	cPoint			point;
};

typedef ed::list<RefPoint> RefPoints;

#endif