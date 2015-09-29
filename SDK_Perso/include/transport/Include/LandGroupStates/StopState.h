#pragma once

#include "LandGroupStates/LandGroupState.h"
#include "CarsEnums.h"

class StopState: public LandGroupState
{
public:
	explicit StopState(LandGroup *host);

	void update(double /*currentTime*/, double /*elapsedTime*/) {};
	LandGroupState *checkNextState() const {return 0;};
	LandGroupStateType type() const {return gtStop;}
};