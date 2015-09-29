#pragma once

#include "AI/Sights/AI_ISight.h"
#include "CockpitBase.h"
#include "Randomizer/RandomObject.h"
#include "wMovingObject.h"

class CannonSight;
class wAircraftGunMount;
class cPosition;

namespace gunner_AI {

class AI_Gunner;

class COCKPITBASE_API SimpleSightAI :  public ISightAI
{
public:
	SimpleSightAI();
	virtual ~SimpleSightAI();

	virtual void l_read(Lua::Config& config, IwoLA* la) override;
	virtual void simulate(double dt) override;
	virtual void guide_gun(double dt) override  = 0;
	virtual void reload() override;

protected:

	void	frendly_fire_check(double dt,wAircraftGunMount* gun_mount);
	void    calculate(const cPosition& gun_mount_pos, woPointer tgt_ptr, double dt);
	void	skill_deviation(double dt);

	double	friendly_fire_check_t_;

	double	skill_deviation_t_; //deviation simulating time
	float rand_dev_az_;
	float rand_dev_el_;

	IwoLA* host_la_;
	CannonSight* sight_;

	RandomObject deviation_rand_; 

};

}