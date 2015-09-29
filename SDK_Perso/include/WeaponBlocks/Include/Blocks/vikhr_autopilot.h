#ifndef __BLOCK_RAMP_H__
#define __BLOCK_RAMP_H__

#include "simulation_block_disc.h"

class wVikhrAutopilotDescriptor;

class wVikhrAutopilot : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wVikhrAutopilot, wSimulationBlockDisc, wVikhrAutopilotDescriptor);
public:
	wVikhrAutopilot(const wVikhrAutopilotDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

protected:
	INPUT_WIRES(7, (
		WIRE(bool, active_),

		WIRE(Math::Vec3d, pos_), 
		WIRE(Math::Vec3d, vel_),
		
		WIRE(Math::Vec3d, coneOrigin_),
		WIRE(Math::Vec3d, coneDir_),
		WIRE(double, coneNearDist_),
		WIRE(double, coneNearRad_)
	));

	OUTPUT_WIRES(2, (
		WIRE(bool, hasSignal_),
		WIRE(Math::Vec3d, Nreq_)
//		WIRE(type, name),
	));

	// Отклонение в метрах
	Math::Vec3d prevDeviation_, intDeviation_;
	Math::Vec3d prevDir_, prevOrigin_;
	double timeLost_;
};

class wVikhrAutopilotDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wVikhrAutopilot)

public:
	virtual wVikhrAutopilot* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
//	DECL_PARAM(type, name);

	DECL_PARAM(double, dt);
	
	DECL_PARAM(double, Kp);
	DECL_PARAM(double, Ki);
	DECL_PARAM(double, Kd);

	DECL_PARAM(double, discreet);
};

#endif
