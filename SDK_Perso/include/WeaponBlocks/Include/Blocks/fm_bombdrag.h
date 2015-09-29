#ifndef __BLOCK_ROTATOR_H__
#define __BLOCK_ROTATOR_H__

#include "fm_bombrocket.h"

class wFMBombDragDescriptor;
class wConstraintRail;

class WEAPON_BLOCKS_API wFMBombDrag : public wFMBombRocket
{
	DECLARE_BLOCK(wFMBombDrag, wFMBombRocket, wFMBombDragDescriptor);
public:
	wFMBombDrag(const wFMBombDragDescriptor* desc, wSimulationSystem* system);

protected:
	virtual Math::Vec3d calcForce_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho);

protected:
	INPUT_WIRES(1, (
		WIRE(bool, par_out_)
	));

	OUTPUT_WIRES(0, ())
};


class WEAPON_BLOCKS_API wFMBombDragDescriptor : public wFMBombRocketDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wFMBombDrag);

public:
	wFMBombDrag* create(Lua::Loader* runtime_cfg,wSimulationSystem* data); 

protected:
	DECL_PARAM(double, cx_factor);

	friend class wFMBombDrag;
};


#endif
