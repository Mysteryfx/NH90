#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Definitions.h"

namespace Radio
{

struct RawObstacle;
struct Obstacle;
struct Path2D;

Distance calcRawObstacleRadius(const RawObstacle & rawObstacle);

Distance calcObstacleRadius(const Obstacle & obstacle);

struct PutReliefPointOnCircle
{
	PutReliefPointOnCircle(Height h1r, Height h2r, Distance rr, Distance R);
	Vec2 operator()(const Vec2 & point) const;
private:
	Distance	rr_;
	Distance	R_;
	Real		dpr_;
	Height		ho_;
};

void makePath2D(Height h1r, Height h2r, Distance rr, Distance R,
				Path2D & path2D);

bool makeRawObstacle(	Height h1r, Height h2r, Distance rr, Distance R, bool cylinder,
						RawObstacle & rawObstacle);

}

#endif _GEOMETRY_H_