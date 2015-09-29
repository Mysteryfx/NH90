#pragma once

#include "AI/AI_Common.h"
#include "AI/States/AI_AnimateModel.h"

namespace gunner_AI {

class GunnerDeadState : public State
{
public:
	virtual void on_start();
};

class GunnerUnloadState : public InitModelState
{
public:
	virtual void on_start();
};


}