#pragma once
#include <osg\Vec2d>
#include "FormationState.h"
#include "CarsEnums.h"

class MovingVehicle;

class ArriveVectorFormationState: public FormationState
{
public:
	ArriveVectorFormationState(MovingVehicle *host, const osg::Vec2d& targetPos, const osg::Vec2d& targetDir);

	void determineForce(osg::Vec2d &steering, double &breaking);

	void update(double /*elapsedTime*/);

	VehicleStateType type() const {return vsArriveVector;}

	double pathParam() const;
	osg::Vec2d arrivedPos() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;
	osg::Vec2d arrivedDir() const {return _targetDir;}
	double desiredSpeed() const;

	bool isArrived() const;
private:
	bool _arrived;

	osg::Vec2d _behindPos;
	int _attemptsCount;
	bool _arrivedBehind;

	osg::Vec2d _targetPos;
	osg::Vec2d _targetDir;
	double _desiredSpeed;
};