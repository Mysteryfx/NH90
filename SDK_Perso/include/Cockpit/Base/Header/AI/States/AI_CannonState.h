#pragma once

#include "AI/States/AI_IState.h"
#include "AI/AI_Common.h"

namespace Lua {class Config;}

namespace gunner_AI {

class CannonState : public State
{
public:

	CannonState();

	virtual void l_read(Lua::Config& config) override;

	virtual bool interruptable() const { return true; }
	virtual void on_start() override;
	virtual void on_finish() override;
	virtual void simulate(double dt) override;


private:

	void draw_gunner_model(bool force_draw = false);

	void  frendly_fire_check();

	float	azimuth_d_max_; //max azimuth deviation
	float	elevation_d_max_; //max elevation deviation
	float	average_burst_time_;

	float	fire_duration_;

	woPointer last_tgt_;

	struct GunnerModel
	{
		GunnerModel() : pylon_(-1), arg_(-1) {}
		char pylon_;
		char arg_;
	} gunner_model_;
};

}