#pragma once
#include <osg\Vec2d>
#include "FormationState.h"
#include "CarsEnums.h"

class MovingVehicle;

class ArrivePositionFormationState: public FormationState
{
public:
	ArrivePositionFormationState(MovingVehicle *host, const osg::Vec2d& targetPos);

	void determineForce(osg::Vec2d &steering, double &breaking);

	void update(double /*elapsedTime*/);

	VehicleStateType type() const {return vsArrivePosition;}

	double pathParam() const;
	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;
	double desiredSpeed() const;

//	bool isArrived() const;
private:
	bool _arrived;

	osg::Vec2d _targetPos;
	double _desiredSpeed;
};