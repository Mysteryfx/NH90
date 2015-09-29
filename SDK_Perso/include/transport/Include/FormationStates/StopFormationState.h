#pragma once
#include <osg\Vec2d>
#include "FormationState.h"
#include "CarsEnums.h"

class MovingVehicle;

class StopFormationState: public FormationState
{
public:
	StopFormationState(MovingVehicle *host);

	void determineForce(osg::Vec2d &steering, double &breaking);

	void update(double /*elapsedTime*/) {};

	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;

	double desiredSpeed() const {return 0.;}
	double pathParam() const;

	VehicleStateType type() const {return vsStop;}
};