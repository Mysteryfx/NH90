#pragma once

#include "LandGroupStates/LandGroupState.h"
#include "CarsEnums.h"

class MovingVehicle;
namespace osg {class Vec2d;}

class ArriveState: public LandGroupState
{
public:
	explicit ArriveState(LandGroup *host);

	void update(double currentTime, double elapsedTime);
	LandGroupState *checkNextState() const;
	LandGroupStateType type() const {return gtArriveEndPos;}
	void onRemoveVehicle(const MovingVehicle* removed);

private:
	bool checkNearLeaderPos(const MovingVehicle* veh, 
		osg::Vec2d& forationPos, osg::Vec2d& formationDir) const;

	bool _stopped;
	double _timeFromEndOfPath;
};