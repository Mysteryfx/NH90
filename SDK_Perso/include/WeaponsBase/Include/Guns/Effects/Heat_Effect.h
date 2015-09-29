#pragma once

#include "Guns/Effects/Gun_Effect.h"
/*#include "Guns/IGun_t_proxy.h"*/

namespace weapons_gun {

class HeatEffectDesc : public Gun_Effect_Desc
{
	friend class HeatEffect;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

protected:
	float shot_k_;
	float barrel_k_;
};

class HeatEffect : public SimulateEffect
{
	friend class HeatEffectDesc;

protected:
	HeatEffect(const HeatEffectDesc* desc, wAircraftGunMount* gun_mount);

public:
#ifdef _ED_HIDDEN_DEBUG 
	~HeatEffect()
	{

	}
#endif

	double*& T_ptr_; //temperature ptr

	const HeatEffectDesc* const desc_;

	virtual void simulate(double dt) override;
	virtual void on_gun_shot();
};

////////////////////////////////////////////////////////////////////////
class HeatEffectExtDesc : public HeatEffectDesc
{
	friend class HeatEffectExt;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:

	float body_k_;
	float fantom_sync_dt_;
};

class HeatEffectExt : public HeatEffect
{
	friend class HeatEffectExtDesc;

protected:
	HeatEffectExt(const HeatEffectExtDesc* desc, wAircraftGunMount* gun_mount);

public:

	double last_upd_time_;
	double*& ext_heat_model_; //ext heat model active flag

	float fantom_sync_dt_;

	virtual void simulate(double dt) override;
	virtual void on_gun_shot();
};
//////////////////////////////////////////////////////////////////////////
class VisualHeatEffectDesc : public HeatEffectDesc
{
	friend class VisualHeatEffect;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
	int arg_;
};

class VisualHeatEffect : public VisualEffect, public HeatEffect
{
	friend class VisualHeatEffectDesc;

	VisualHeatEffect(const VisualHeatEffectDesc* desc, wAircraftGunMount* gun_mount);

public:
	virtual void draw_update() override;

private:
	int arg_;
};


}
