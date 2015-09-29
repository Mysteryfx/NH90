#pragma once

#include "LandGroupStates/LandGroupState.h"
#include "CarsEnums.h"

#include <ed/string.h>

class MovingVehicle;
class CustomFormation;

namespace osg {class Vec2d;}

class ArriveCustomFormState: public LandGroupState
{
public:
	explicit ArriveCustomFormState(LandGroup *host, double delay, const ed::string& templateId);

	void update(double currentTime, double elapsedTime);
	LandGroupState *checkNextState() const;
	LandGroupStateType type() const {return gtArriveCustomForm;}
	void onRemoveVehicle(const MovingVehicle* removed);

private:
	bool _stopped;
	double _timeFromStart;
	double _delay;

	void moveGroupLeader(const CustomFormation*);
};