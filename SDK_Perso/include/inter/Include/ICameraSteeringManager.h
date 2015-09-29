#pragma once

#include "Inter.h"
#include "cLinear.h"

class INTER_API ICameraSteeringManager
{
public:
	virtual ~ICameraSteeringManager() {}

	virtual void deactivate() = 0;
	virtual void CommandSet(int cmd, float value) = 0;
	virtual void CommandSet(int cmd) = 0;
	virtual void updateSteeringPoint() = 0;
};

extern INTER_API ICameraSteeringManager* globalCameraSteeringManager;
