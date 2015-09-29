#pragma once
#include <osg\Vec2d>
#include "FormationState.h"
#include "CarsEnums.h"

class TrailVehicle;
class LeaderVehicle;

class LeaderFormationState: public FormationState
{
public:
	LeaderFormationState(MovingVehicle *host);

	void determineForce(osg::Vec2d &steering, double &breaking);

	VehicleStateType type() const {return vsLeader;}

	void update(double /*elapsedTime*/) {};

	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;
	double desiredSpeed() const;
	double pathParam() const;

private:
	const LeaderVehicle* _leader;
};