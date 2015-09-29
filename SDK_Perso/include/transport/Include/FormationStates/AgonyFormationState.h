#pragma once
#include <osg\Vec2d>
#include "FormationState.h"
#include "CarsEnums.h"

class MovingVehicle;

class AgonyFormationState: public FormationState
{
public:
	AgonyFormationState(MovingVehicle *host);

	void determineForce(osg::Vec2d &steering, double &breaking);

	void update(double /*elapsedTime*/);

	VehicleStateType type() const {return vsAgony;}

	double pathParam() const;
	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;
	double desiredSpeed() const;

private:
	double _steerAngle, _speedCoeff, _breakingCoeff;
};