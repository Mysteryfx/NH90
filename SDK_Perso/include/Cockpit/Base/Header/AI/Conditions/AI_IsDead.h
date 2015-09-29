#pragma once

#include "AI/Conditions/AI_ICondition.h"

namespace gunner_AI {

class IsDead : public Condition
{
public: 
	virtual bool operator()(const AI_Gunner* host) const override;
};

class IsPresent : public Condition
{
public: 

	virtual bool l_read(Lua::Config& config) override;
	virtual bool operator()(const AI_Gunner* host) const override;

private:
	bool value_;
};

}