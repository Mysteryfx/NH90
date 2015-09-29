#pragma once

#include "AI/AI_Factory.h"
#include "cAlgorithm.h"

namespace Lua {
	class Config;
}

class IwoLA;

namespace gunner_AI {

class AI_Gunner;

class ISightAI
{
public:

	ISightAI() : deviation_az_(0.0f), deviation_el_(0.0f), 
		gunner_(nullptr), pylon_(-1), gun_num_(-1),
		effective_fire_dist_(500.0f), friendly_fire_(true) {}

	virtual ~ISightAI(){}

	void set_gunner(AI_Gunner* gunner) { gunner_ = gunner; }
	AI_Gunner* get_gunner() const { return gunner_; }

	char get_pylon() const { return pylon_; }

	virtual void l_read(Lua::Config& config, IwoLA* la){}
	virtual void simulate(double dt) = 0;

	virtual void guide_gun(double dt) = 0;
	virtual void reload() = 0;

	float get_azimuth_d() const { return deviation_az_; }
	float get_elevation_d() const { return deviation_el_; }
	float get_eff_dist() const { return effective_fire_dist_; }

	const r_to_r_f& get_azimuth_limits() const { return azimuth_; }
	const r_to_r_f& get_elevation_limits() const { return elevation_; }

	bool  friendly_fire() const { return friendly_fire_; } 

protected:

	float deviation_az_;
	float deviation_el_;
	float effective_fire_dist_;

	r_to_r_f azimuth_; /* in rad */
	r_to_r_f elevation_;  /* in rad */

	char pylon_; /*0 - on board. 1...n*/
	char gun_num_;

	bool friendly_fire_; //friendly fire probability

	AI_Gunner* gunner_;
};

typedef AIFactory<ISightAI>	IGunSightFactory;
IGunSightFactory::Fmap IGunSightFactory::fabric;

#define REGISTER_GUN_SIGHT_AI_FACTORY(class_n) static AICreator<ISightAI,##class_n##,IGunSightFactory> reg##class_n(#class_n);


}