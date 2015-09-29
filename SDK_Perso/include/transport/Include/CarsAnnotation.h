#pragma  once

#ifdef _OPENSTEER

#include "Annotation.h"
#include "World.h"

inline void annotationLine(const osg::Vec2d& startPoint, const osg::Vec2d& endPoint, const osg::Vec3d& color)
{
	OpenSteer::Annotation::annotationLine(osg::toVec3d(startPoint), osg::toVec3d(endPoint), color);
}

inline void annotationCircleOrDisk(const double radius, const osg::Vec2d& center,
							  const osg::Vec3d& color, const int segments, const bool filled,
							  const bool in3d)  
{
	OpenSteer::Annotation::annotationCircleOrDisk(radius, osg::upVD, osg::toVec3d(center), color, segments, filled, in3d);
}

inline void annotationVelocityAcceleration(const SimpleVehicle* host) 
{
	OpenSteer::Annotation::annotationVelocityAcceleration(host);
}

inline void annotateFollowPath(const MovingVehicle*, const osg::Vec2d& targetPos, const osg::Vec2d& force, double) 
{
	OpenSteer::Annotation::annotateFollowPath(targetPos, force);
}

inline void annotateAvoidObstacle(const SimpleVehicle* host, const Utils::Edges& sensors, const Utils::PathIntersection &nearest, 
								  const osg::Vec2d& steer) 
{
	OpenSteer::Annotation::annotateAvoidObstacle(host, sensors, nearest, steer);
}

inline void annotateAvoidNeighborX(const SimpleVehicle* host, 
							  const SimpleVehicle& threat,
							  const double steer,
							  const char *msg) 
{
	OpenSteer::Annotation::annotateAvoidNeighborX(host, threat, steer, msg);
}

inline void annotateAvoidCloseNeighbor(const SimpleVehicle* host, 
								  const SimpleVehicle& other, const double additionalDistance) 
{
	OpenSteer::Annotation::annotateAvoidCloseNeighbor(host, other, additionalDistance);
}

inline void annotateForce(const SimpleVehicle* host, const osg::Vec2d& force, bool dottedLine) 
{
	OpenSteer::Annotation::annotateForce(host, force, dottedLine);
}

inline void annotateVehicle(const MovingVehicle& veh)
{

}

#else

#include <osg/Vec2d>
#include <osg/Vec3d>
class SimpleVehicle;
class MovingVehicle;
class LeaderVehicle;

namespace Utils
{
	struct PathIntersection;
	struct Edge;
	class VelocityObstacles;
}

extern bool carsAnnotationOn;

inline void annotationLine(const osg::Vec2d&, const osg::Vec2d&, const osg::Vec2d&){}

inline void annotationCircleOrDisk(const double, const osg::Vec2d&,
							  const osg::Vec3d&, const int, const bool,
							  const bool)  {}

inline void annotationVelocityAcceleration(const SimpleVehicle* ) {}

void annotateFollowPath(const MovingVehicle*, const osg::Vec2d&, const osg::Vec2d&, double, const char *);

void annotateAvoidNeighbors(const MovingVehicle*, const Utils::VelocityObstacles& vos, 
							const osg::Vec2d&, const osg::Vec2d&);

void annotateAvoidObstacle(const SimpleVehicle*, const Utils::Edge*, int, double, 
						   const Utils::PathIntersection&, const osg::Vec2d&);
void annotateForce(const SimpleVehicle* host, const osg::Vec2d& force, 
				   const osg::Vec2d& adjustedForce, double breaking);
void annotateVehicle(const MovingVehicle& veh);
void annotateLeaderVehicle(const LeaderVehicle* veh);

#endif // _OPENSTEER
