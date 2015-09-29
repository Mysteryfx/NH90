#pragma once

#include "LandGroupStates/LandGroupState.h"
#include "LandGroup.h"
#include "CarsEnums.h"

class MovingVehicle;

namespace osg {class Vec2d;}

class DisperseState: public LandGroupState
{
public:
	explicit DisperseState(LandGroup *host, double stopDelay);

	void update(double currentTime, double elapsedTime);
	LandGroupState *checkNextState() const;
	LandGroupStateType type() const {return gtDisperse;}

private:
	bool _arrived;
	double _elapsedTime;
	double _stopDelay;
	ed::vector<double> _stopedVec;
};
