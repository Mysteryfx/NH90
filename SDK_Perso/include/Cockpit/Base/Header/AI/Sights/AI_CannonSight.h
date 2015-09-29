#pragma once

#include "AI/Sights/AI_SimpleSight.h"

namespace gunner_AI {

class CannonSightAI : public SimpleSightAI
{
public:

	virtual void l_read(Lua::Config& config, IwoLA* la) override;
	virtual void simulate(double dt) override;
	virtual void guide_gun(double dt) override;

private:
	int az_arg_; 
	int elev_arg_;
};

}