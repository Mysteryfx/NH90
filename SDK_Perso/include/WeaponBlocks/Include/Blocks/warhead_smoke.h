#ifndef __WARHEAD_SMOKE_BLOCK_H__
#define __WARHEAD_SMOKE_BLOCK_H__

#include "simulation_block_disc.h"

class wWarheadSmokeBlockDescriptor;

class wWarheadSmokeBlock : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wWarheadSmokeBlock, wSimulationBlockDisc, wWarheadSmokeBlockDescriptor);
public:
	wWarheadSmokeBlock(const wWarheadSmokeBlockDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double) { return -1; };

protected:
	void explode_(bool);

protected:
	INPUT_WIRES(3, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Vec3d, vel_),
		WIRE(int, object_id_)
	))

	OUTPUT_WIRES(0, ())

	INPUT_PORT(bool, explode_port_);
};


class wWarheadSmokeBlockDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wWarheadSmokeBlock);

public:
	wWarheadSmokeBlock* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(Math::Vec3d, color);
	DECL_PARAM(double, transparency);
	DECL_PARAM(double, intensity);
	DECL_PARAM(double, duration);
};


#endif
