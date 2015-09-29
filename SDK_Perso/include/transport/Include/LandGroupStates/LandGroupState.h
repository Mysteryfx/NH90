#pragma once

#include "Mem/Shared.h"
#include <functional>

class LandGroup;
class MovingVehicle;
enum LandGroupStateType;

class LandGroupState: public Mem::Shared
{
public:
	LandGroupState(LandGroup *host)
		:_host(host){}

	virtual void update(double currentTime, double elapsedTime) = 0;
	virtual LandGroupState *checkNextState() const = 0;
	virtual LandGroupStateType type() const = 0;

	virtual void onRemoveVehicle(const MovingVehicle* removed) {};
	virtual void updateOnFormation() {};

protected:
	LandGroup *_host;
};

class updateFormationLinks: public std::binary_function<MovingVehicle*, const MovingVehicle*, void>
{
public:
	void operator() (MovingVehicle *veh, const MovingVehicle* removedVeh) const;
};