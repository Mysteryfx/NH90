#pragma once
#include <osg\Vec2d>
#include "TrailLink.h"
#include "FormationState.h"
#include "CarsEnums.h"

class MovingVehicle;
class TrailVehicle;
class LeaderVehicle;

class FollowerFormationState: public FormationState
{
public:
	FollowerFormationState(MovingVehicle *host);

	void determineForce(osg::Vec2d &steering, double &breaking);

	void update(double /*elapsedTime*/);

	VehicleStateType type() const {return vsFollower;}

	double pathParam() const;
	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;
	double desiredSpeed() const;
	bool isPursuitEndTrail() const { return _isPursuitEndTrail; };

private:
	const VehicleLink& _trailLink;
	double _pathParam;
	double _desiredSpeed;
	osg::Vec2d _arrivedPos;
	bool _isPursuitEndTrail;
};
