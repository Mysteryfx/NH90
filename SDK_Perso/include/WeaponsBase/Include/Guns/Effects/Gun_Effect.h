#pragma once

#include "Guns/Effects/Effect_Factory.h"
#include "Mem/Shared.h"

namespace Lua { class Config; }

class wAircraftGunMount;

namespace weapons_gun {

class GunEffect;

class Gun_Effect_Desc
{
public:
	virtual void l_read(Lua::Config& config) = 0;
	virtual void create(wAircraftGunMount* gun_mount) = 0;
};

class GunEffect : public Mem::SharedLite
{
public:
	virtual ~GunEffect(){}

protected:

	GunEffect(wAircraftGunMount* gun_mount) : gun_mount_(gun_mount) {}
	wAircraftGunMount* gun_mount_;
};

class SimulateEffect : public virtual GunEffect
{
public:
	SimulateEffect(wAircraftGunMount* mount);
	virtual void simulate(double dt) = 0;
};

typedef Mem::Ptr<SimulateEffect> SimulateEffectPtr;

class VisualEffect : public virtual GunEffect
{
public:
	VisualEffect(wAircraftGunMount* mount);
	virtual void draw_update() = 0;
};

typedef Mem::Ptr<VisualEffect> VisualEffectPtr;

typedef Factory<Gun_Effect_Desc> EffectFactory;
EffectFactory::Fmap EffectFactory::fabric;

#define REGISTER_GUN_EFFECT_FACTORY(class_n) static Creator<Gun_Effect_Desc,class_n,EffectFactory> reg##class_n(#class_n);

}