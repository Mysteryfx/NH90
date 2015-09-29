#ifndef __CBU_CONTROL_BLOCK_H__
#define __CBU_CONTROL_BLOCK_H__

#include "simulation_block_disc.h"

#ifndef NO_REAL_LOCKON
#include "lPointOnSurface.h"
#include "edTerrain/PointOnSurfaceAdapter.h"
#endif



class wCbuControlBlockDescriptor;

class wCbuControlBlock : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wCbuControlBlock, wSimulationBlockDisc, wCbuControlBlockDescriptor);
public:
	wCbuControlBlock(const wCbuControlBlockDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

	double height;

protected:

	INPUT_WIRES(2, (
		WIRE(Math::Vec3d, pos_),
		WIRE(Math::Rot3d, rot_)
	));

	OUTPUT_WIRES(2, (
		WIRE(bool, check_obj_),
		WIRE(Math::Vec3d, des_omega_)
	));

	OUTPUT_PORT(bool,  open_);
	INPUT_PORT_CONST(double, open_height_);
	INPUT_PORT_CONST(Math::Vec3d, desired_omega_);
	INPUT_PORT_CONST(bool, fzu_39_);
	INPUT_PORT_CONST(double, time_delay_);

	

#ifndef NO_REAL_LOCKON
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface surf_;
#else 
	edterrain::PointOnSurfaceAdapter surf_;
#endif
#endif
};

class wCbuControlBlockDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wCbuControlBlock)

public:
	virtual wCbuControlBlock* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

	DECL_PARAM(double, dt);
	DECL_PARAM(double, check_delay);
};

#endif
