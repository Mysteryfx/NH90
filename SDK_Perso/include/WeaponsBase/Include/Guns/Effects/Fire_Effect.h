#pragma once

#include "Guns/Effects/Gun_Effect.h"

namespace Effects {
	class CannonLight; }

namespace weapons_gun
{

class FireEffectDesc : public Gun_Effect_Desc
{
	friend class FireEffect;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
	int	   arg_;
	float  duration_;
	float  animation_length_;
	float  attenuation_;
};

class FireEffect : public VisualEffect, public SimulateEffect
{
	friend class FireEffectDesc;

public:
	virtual void draw_update() override;
	virtual void simulate(double dt) override;

	~FireEffect();

protected:

	FireEffect(const FireEffectDesc* desc, wAircraftGunMount* gun_mount);

	void on_gun_shot();
	void on_fire_stop();

	void create_light(const FireEffectDesc* desc);

	//////////////////////////////////////////////////////////////////////////

	Effects::CannonLight* light_;

	int	  arg_;
	float value_;
	float animation_length_;
	unsigned char phase_count_; 

	double	duration_;
	double	start_time_;
	unsigned char phase_;
	unsigned char next_phase_;
};

}