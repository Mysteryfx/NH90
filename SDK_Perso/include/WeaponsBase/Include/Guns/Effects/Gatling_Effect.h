#pragma once

#include "Guns/Effects/Gun_Effect.h"

namespace weapons_gun {

class GatlingEffectDesc : public Gun_Effect_Desc
{
	friend class GatlingEffect;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;
private:
	int arg_;
	unsigned char barrels_n_;
	float spin_up_t_;
	float spin_down_t_;
};

class GatlingEffect : public SimulateEffect, public VisualEffect
{
	friend class GatlingEffectDesc;

protected:
	GatlingEffect(const GatlingEffectDesc* desc, wAircraftGunMount* gun_mount);

public:
	virtual void simulate(double dt) override;
	virtual void draw_update() override;

protected:
	float omega_rel_;

private:
	const GatlingEffectDesc* desc_;
	float value_;
	double prev_ef_time_;
};

//////////////////////////////////////////////////////////////////////////
class A10C_GatlingEffectDesc : public GatlingEffectDesc
{
	friend class A10C_GatlingEffect;
public:
	virtual void create(wAircraftGunMount* gun_mount) override;
private:
};

class A10C_GatlingEffect : public GatlingEffect
{
	friend class A10C_GatlingEffectDesc;

protected:
	A10C_GatlingEffect(const A10C_GatlingEffectDesc* desc, wAircraftGunMount* gun_mount);

public:
	virtual void simulate(double dt) override;
	virtual void draw_update() override;

};

}