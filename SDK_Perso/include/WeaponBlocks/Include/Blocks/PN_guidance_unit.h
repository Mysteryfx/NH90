#ifndef __PN_GUIDANCE_H__
#define __PN_GUIDANCE_H__

#include "Math/Vector.h"

#include "simulation_block.h"

class wPNGuidanceUnitDescriptor;

class wPNGuidanceUnit : public wSimulationBlockStateless
{
	DECLARE_BLOCK(wPNGuidanceUnit, wSimulationBlockStateless, wPNGuidanceUnitDescriptor);

public:
	wPNGuidanceUnit(wPNGuidanceUnitDescriptor* desc);

	virtual void update(double time, lead_data_ptr_t data);

protected:
	INPUT_WIRES(2, (
		WIRE(Math::Vec3d, omegaLOS_),
		WIRE(Math::Vec3d, vel_)
	))

	OUTPUT_WIRES(1, (
		WIRE(Math::Vec3d, Ac_)
	))
	const Math::Vec3d AcMin_;
	const Math::Vec3d AcMax_;
};

class wPNGuidanceUnitDescriptor : public wSimulationBlockStatelessDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wPNGuidanceUnit);
public:
	virtual wPNGuidanceUnit* create(Lua::Loader* runtime_cfg);

public:
	DECL_PARAM(double, NR);
	DECL_PARAM(double, AcLim);
};

#endif //__PN_GUIDANCE_H__
