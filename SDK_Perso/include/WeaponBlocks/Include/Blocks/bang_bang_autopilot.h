#ifndef __BLOCK_BANG_BANG_AUTOPILOT_
#define __BLOCK_BANG_BANG_AUTOPILOT_

#include "simulation_block_disc.h"

#include "Math/PID.h"

class wBangBangAutopilotDescriptor;

class wBangBangAutopilot : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wBangBangAutopilot, wSimulationBlockDisc, wBangBangAutopilotDescriptor);

public:
	wBangBangAutopilot(wBangBangAutopilotDescriptor* desc, wSimulationSystem* system);
	~wBangBangAutopilot();

	virtual double simulate(double time);

protected:
	INPUT_WIRES(4, (
		WIRE(bool,			hasSignal_),
		WIRE(Math::Vec3d,	error_),
		WIRE(bool,			elecPower_),
		WIRE(Math::Vec3d,	omega_)
	))
	OUTPUT_WIRES(2, (
		WIRE(bool,			signal_),
		WIRE(Math::Vec3d,	fins_)
	))
	bool					hadSignal;
};

class wBangBangAutopilotDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wBangBangAutopilot);
public:
	virtual wBangBangAutopilot* create(Lua::Loader* runtime_cfg, wSimulationSystem* data);
	DECL_PARAM(double,		dt);
	DECL_PARAM(double,		omegaDumpingK);
};

#endif __BLOCK_BANG_BANG_AUTOPILOT_
