#pragma once
#include <osg\Vec2d>
#include "FormationState.h"
#include "CarsEnums.h"

class MovingVehicle;

class WanderFormationState: public FormationState
{
public:
	WanderFormationState(MovingVehicle *host);

	void determineForce(osg::Vec2d &steering, double &breaking);

	void update(double /*elapsedTime*/);

	VehicleStateType type() const {return vsWander;}

	double pathParam() const;
	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;
	double desiredSpeed() const;

private:
	void initRandParams();
	
	double _timeFromInit;
	osg::Vec2d _targetDir;
	double _targetSpeed;
};