#ifndef __ATM_SAMPLER_H__
#define __ATM_SAMPLER_H__

#include "Math/Vector.h"

#include "simulation_block.h"
#include "lPointOnSurface.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

class wAtmSamplerDescriptor;

class wAtmSampler : public wSimulationBlockStateless
{
	DECLARE_BLOCK(wAtmSampler, wSimulationBlockStateless, wAtmSamplerDescriptor);

public:
	wAtmSampler(wAtmSamplerDescriptor* desc);

	virtual void update(double time, lead_data_ptr_t data);

protected:
	INPUT_WIRES(1, (
		WIRE(Math::Vec3d, pos)
	))

	OUTPUT_WIRES(3, (
		WIRE(double, rho),
		WIRE(double, M),
		WIRE(Math::Vec3d,  wind)
	))

	bool   isNewAtmosphere;
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface surf_;
#else 
	edterrain::PointOnSurfaceAdapter surf_;
#endif
};

class wAtmSamplerDescriptor : public wSimulationBlockStatelessDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wAtmSampler);
public:
	virtual wAtmSampler* create(Lua::Loader* runtime_cfg);
};

#endif
