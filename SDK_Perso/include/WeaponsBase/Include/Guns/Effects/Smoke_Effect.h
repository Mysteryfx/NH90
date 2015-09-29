#pragma once

#include "Guns/Effects/Gun_Effect.h"
#include "graphicEffect.h"

namespace Effects
{
	struct IGraphicEffectParamBlock;
}

namespace weapons_gun
{

class SmokeEffectDesc : public Gun_Effect_Desc
{
	friend class SmokeEffect;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
};

class SmokeEffect : public SimulateEffect
{
	friend class SmokeEffectDesc;

public:

	virtual void simulate(double dt) override{}

	~SmokeEffect();

protected:

	SmokeEffect(const SmokeEffectDesc* desc, wAircraftGunMount* gun_mount);

	gePointer smoke_ptr_;
	Effects::IGraphicEffectParamBlock *smokePB_;

	double	   last_time_;

	void on_gun_shot();
	void on_fire_stop();

};

}