#ifndef __CLUSTER_LAUNCHER_H__
#define __CLUSTER_LAUNCHER_H__

#include "simulation_block_disc.h"

class wBlockClusterLauncherDescriptor;

class wBlockClusterLauncher : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wBlockClusterLauncher, wSimulationBlockDisc, wBlockClusterLauncherDescriptor);
public:
	wBlockClusterLauncher(const wBlockClusterLauncherDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

protected:
	void onFire_(bool);

protected:
	INPUT_WIRES(7, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
		WIRE(Math::Rot3d, rot_),
		WIRE(Math::Vec3d, omega_),

		WIRE(int, country_),
        WIRE(int, launcher_id_),
		WIRE(int, state_)
	))

	OUTPUT_WIRES(0, (
//		WIRE(type, name),
	))

	INPUT_PORT(bool, fire_);
};

class wAmmunitionDescriptor;

class wBlockClusterLauncherDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wBlockClusterLauncher)

public:
	virtual wBlockClusterLauncher* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

	const wAmmunitionDescriptor* getClusterDesc() const { return cluster_; }

	virtual void serialize(wBlockSerializer& ser);

public:
//	DECL_PARAM(type, name);
	DECL_PARAM(bool, is_server);
protected:
	const wAmmunitionDescriptor* cluster_;
};

#endif
