#pragma once

#include "LandGroupStates/LandGroupState.h"
#include "CarsEnums.h"

class WaitState: public LandGroupState
{
public:
	WaitState(LandGroup *host, double delay);

	void update(double /*currentTime*/, double elapsedTime);
	LandGroupState *checkNextState() const;
	LandGroupStateType type() const {return gtWait;}
private:
	double _delay;
	double _timeFromStart;
};