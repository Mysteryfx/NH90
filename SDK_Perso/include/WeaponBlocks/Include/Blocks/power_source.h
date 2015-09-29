#ifndef __ATM_SAMPLER_H__
#define __ATM_SAMPLER_H__

#include "Math/Vector.h"

#include "simulation_block.h"

class wPowerSourceDescriptor;

class wPowerSource : public wSimulationBlockStateless
{
	DECLARE_BLOCK(wPowerSource, wSimulationBlockStateless, wPowerSourceDescriptor);

public:
	wPowerSource(wPowerSourceDescriptor* desc);

	virtual void update(double time, lead_data_ptr_t data);

protected:
	INPUT_WIRES(0, ( ))

	OUTPUT_WIRES(1, (
		WIRE(bool, elecPower_)
	))
};

class wPowerSourceDescriptor : public wSimulationBlockStatelessDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wPowerSource);
public:
	virtual wPowerSource* create(Lua::Loader* runtime_cfg);
	DECL_PARAM(double, workTimeMax);
};

#endif
