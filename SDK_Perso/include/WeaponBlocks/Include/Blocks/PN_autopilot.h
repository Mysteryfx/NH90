#ifndef __BLOCK_PN_AUTOPILOT_H__
#define __BLOCK_PN_AUTOPILOT_H__

#include "simulation_block_disc.h"

class wPNAutopilotDescriptor;

class wPNAutopilot : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wPNAutopilot, wSimulationBlockDisc, wPNAutopilotDescriptor);
public:
	wPNAutopilot(const wPNAutopilotDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

protected:
	INPUT_WIRES(4, (
		WIRE(bool,			active_),
		WIRE(Math::Vec3d,	omega_),
		WIRE(Math::Vec3d,	An_),
		WIRE(Math::Vec3d,	Ac_)
	));

	OUTPUT_WIRES(1, (
		WIRE(Math::Vec3d,	fins_)
	));
	Math::Vec3d				integrator;
};

class wPNAutopilotDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wPNAutopilot)

public:
	virtual wPNAutopilot* create(Lua::Loader* runtime_cfg, wSimulationSystem* data);

public:
	DECL_PARAM(double,		dt);
	DECL_PARAM(double,		Ka);
	DECL_PARAM(double,		Iarm);
	DECL_PARAM(double,		K);	
	DECL_PARAM(double,		Ki);
	DECL_PARAM(double,		Kg);
	DECL_PARAM(double,		finsLimit);
};

#endif __BLOCK_PN_AUTOPILOT_H__
