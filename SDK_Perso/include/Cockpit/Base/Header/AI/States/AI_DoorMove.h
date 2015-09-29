#pragma once

#include "AI/States/AI_IState.h"

namespace gunner_AI {

class DoorMoveState : public State
{
public:
	virtual void on_start();
};

}