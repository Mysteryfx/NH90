#ifndef __BLOCK_RAMP_H__
#define __BLOCK_RAMP_H__

#include "simulation_block_disc.h"

class wWarheadPuffDescriptor;

class wWarheadPuff : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wWarheadPuff, wSimulationBlockDisc, wWarheadPuffDescriptor);
public:
	wWarheadPuff(const wWarheadPuffDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time) { return -1; };

protected:
	void onFire_(bool);

protected:
	INPUT_WIRES(2, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_)
	))

	OUTPUT_WIRES(0, (
//		WIRE(type, name),
	))

	INPUT_PORT(bool, fire_);
};

class wWarheadPuffDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wWarheadPuff)

public:
	virtual wWarheadPuff* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
//	DECL_PARAM(type, name);

	DECL_PARAM(double, scale);
};

#endif
