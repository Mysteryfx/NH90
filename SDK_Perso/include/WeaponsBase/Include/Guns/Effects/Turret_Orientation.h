#pragma once

#include "Guns/Effects/Gun_Effect.h"
#include <tuple>

namespace weapons_gun
{

class TurretOrietationDesc : public Gun_Effect_Desc
{
	friend class TurretOrietation;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
	int   azimuth_arg;
	int   elevation_arg;
	float azimuth_b;
	float azimuth_k;
	float elevation_b;
	float elevation_k;

};

class TurretOrietation : public SimulateEffect
{

	const TurretOrietationDesc * desc_;
	friend class TurretOrietationDesc;
public:
	TurretOrietation(const TurretOrietationDesc* desc, wAircraftGunMount* gun_mount);
	virtual void simulate(double dt) override;
};

}
