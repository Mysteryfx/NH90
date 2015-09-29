#pragma once

#include "Guns/Effects/Gun_Effect.h"

namespace weapons_gun
{

class GunTriggerDesc : public Gun_Effect_Desc
{
	friend class GunTrigger;
public:
	virtual void l_read(Lua::Config& config) override {}
	virtual void create(wAircraftGunMount* gun_mount) override;
};

class GunTrigger : public SimulateEffect
{
	friend class GunTriggerDesc;
public:
	virtual void simulate(double dt) override;

protected:
	GunTrigger(wAircraftGunMount* gun_mount);
};


}