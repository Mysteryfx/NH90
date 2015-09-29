#pragma once

#include "WorldGeneral.h"


class IwHumanPilot
{
public:
	virtual ~IwHumanPilot() {}
	virtual void SetCommand(int) = 0;
	virtual void SetCommand(int, float) = 0;
	virtual bool IsLocalUser() = 0;
};

extern WORLDGENERAL_API IwHumanPilot *globalHumanPilot;
