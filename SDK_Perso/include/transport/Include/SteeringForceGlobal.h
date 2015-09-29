#pragma once
#include <osg\Vec2d>
#include "TrailLink.h"
#include "Obstacle.h"
#include "Vehicles\MovingVehicle.h"

class ObstacleGroup;

//////////////////////////////////////////////////////////////////////////
//				              Steering                                 //
osg::Vec2d steerForSeek(const MovingVehicle* host, const osg::Vec2d& target);

osg::Vec2d steerTowardHeading (const MovingVehicle* host, const osg::Vec2d& desiredGlobalHeading);

void steerToSetDirection(const MovingVehicle* host, const osg::Vec2d& dir, 
						 osg::Vec2d& steeringForce, double& breakingForce);

void steerForTargetSpeed(const MovingVehicle* host, const double targetSpeed, 
						 osg::Vec2d& steeringForce, double& breakingForce);

void steerForArrive(const osg::Vec2d& target, double& targetSpeed, const osg::Vec2d& targetDir, 
					const MovingVehicle* host, osg::Vec2d& steeringForce, double& breakingForce);

void steerToOffsetPursuit(const VehicleLink& link, const MovingVehicle* host,
						  double &agregateSpeed, osg::Vec2d &agregateTargetPos, 
						  osg::Vec2d& steeringForce, double& breakingForce);

void steerToFollowPath(const VehicleLink& link, const MovingVehicle* host,
					   double &agregateSpeed, osg::Vec2d &agregateTargetPos, 
					   osg::Vec2d& steeringForce, double& breakingForce);

void steerToPursuitEndTrail(const VehicleLink& link, const MovingVehicle* host,
							double &agregateSpeed, osg::Vec2d &agregateTargetPos, 
							osg::Vec2d& steeringForce, double& breakingForce);

void steerToAvoidNeighbors(const MovingVehicle* host, const Vehicles& neighbors, 
							osg::Vec2d& steeringForce, double& breakingForce);

void steerToAvoidObstacles(const MovingVehicle* host, ObstacleGroup& obstacles, osg::Vec2d& steeringForce, 
						   double& breakingForce, osg::Vec2d &obsDir, osg::Vec2d &avoidDir);


//////////////////////////////////////////////////////////////////////////
//				              Utils for steering                        //

namespace Utils
{
	struct PathIntersection{
		
		PathIntersection()
			:intersect(false), distance(DBL_MAX)
		{
		}

		bool intersect;
		int edge;
		double distance;
		osg::Vec2d surfacePoint;
		osg::Vec2d surfaceNormal;
		Mem::Ptr<Obstacle> obstacle;
		double alpha;
	};

	struct VelocityObstacle
	{
		osg::Vec2d verts[4];
		const MovingVehicle* host;
	};
	class VelocityObstacles: public ed::vector<VelocityObstacle> {};

	bool getVelocityObstacles(const MovingVehicle *host, const Vehicles& neighbors, double maxDist, VelocityObstacles& vos);
	osg::Vec2d getAvoidVelocity(const MovingVehicle *host, const VelocityObstacles& vos);
	bool isInVelocityObtacles(const osg::Vec2d& p, const VelocityObstacles& vos);
	void checkPassingOtherColumn(const MovingVehicle* host, const Vehicles& neighbors);

	void steerForArriveWithSpeed(const osg::Vec2d& target, const osg::Vec2d& targetDir, double targetSpeed, 
		const MovingVehicle* host, osg::Vec2d& steeringForce, double& breakingForce, double* arriveSpeed);

	void steerIfBeforeForerunner(double pathParam, const Trail* trail, const MovingVehicle* host, 
		double &agregateSpeed, osg::Vec2d &agregateTargetPos, 
		osg::Vec2d& steeringForce, double& breakingForce);

	double predictNearestApproachTimeX(const MovingVehicle* host, const MovingVehicle* other, double offset);
	
	double predictNearestApproachTimeRect(const MovingVehicle* host, const MovingVehicle* other, double offset, double minTime);
	double predictNearestApproachTimeProj(const MovingVehicle* host, const MovingVehicle* other, double offset, const osg::Vec2d& n);
	void getRectProjection(const MovingVehicle* veh, bool halfrect, const osg::Vec2d& n, double &left, double &right);

	void findIntersectionWithObstacle(const Edge& sensor, Mem::Ptr<Obstacle> obs, PathIntersection& intersection);
	Obstacle::AvoidDir defineAvoidDir(Mem::Ptr<Obstacle> obs, const ObstacleGroup& allObstacles, const MovingVehicle* host);
	bool isObsAdjoining(Mem::Ptr<Obstacle> o1, Mem::Ptr<Obstacle> o2);
}