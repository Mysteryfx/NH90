#pragma  once

#include "LandGroup.h"

#ifdef _OPENSTEER

#include "World.h"

inline void getNeighbors(const osg::Vec2d& center, double maxRadius, Vehicles& vehicles)
{
	return World::instance()->getNeighbors(center, maxRadius, vehicles);
}

inline void getAllObstacles(const osg::Vec2d& center, double maxRadius, ObstacleGroup& obstacles)
{
	return World::instance()->getObstacles(center, maxRadius, obstacles);
}

inline void getObstacles(const osg::Vec2d& center, double maxRadius, ObstacleGroup& obstacles, RideOnObstacles& /*rideOnObs*/, const MovingVehicle* /* veh*/)
{
	return World::instance()->getObstacles(center, maxRadius, obstacles);
}

inline double drand(double a, double b)
{
	return a + ((double)rand()/(double)RAND_MAX)*(b-a);
}

inline void notifyDead(const MovingVehicle* /*deathVehicle*/) {}
bool canDisperse(const LandGroup::MovingVehicles&);

#else

#include <osg/Vec2d>
class MovingVehicle;
class ObstacleGroup;
class RideOnObstacles;
class Vehicles;
class woMovingVehicle;


void getNeighbors(const osg::Vec2d& center, double maxRadius, Vehicles& vehicles);
void getAllObstacles(const osg::Vec2d& center, double maxRadius, ObstacleGroup& obstacles);
void getObstacles(const osg::Vec2d& center, double maxRadius, ObstacleGroup& obstacles, RideOnObstacles& rideOnObs, const MovingVehicle* veh);
void notifyDead(const MovingVehicle* deathVehicle);
void notifyOtherGroupDrawback(const LandGroup* myGroup, const LandGroup* otherGroup);

bool canDisperse(const LandGroup::MovingVehicles&);

#include "cLinear.h"
namespace osg
{
	template<class Vec>
	inline cVector fromVec3d(const Vec& v)
	{
		return cVector(v.x(), v.y(), v.z());
	}

	template<class Vec>
	inline dVector d_fromVec3d(const Vec& v)
	{
		return dVector(v.x(), v.y(), v.z());
	}

	template<class Vec>
	inline cVector fromVec2d(const Vec& v)
	{
		return cVector(v.x(), 0, v.y());
	}

	template<class Vec>
	inline dVector d_fromVec2d(const Vec& v)
	{
		return dVector(v.x(), 0, v.y());
	}
}

double drand(double a, double b);

#endif // _OPENSTEER
