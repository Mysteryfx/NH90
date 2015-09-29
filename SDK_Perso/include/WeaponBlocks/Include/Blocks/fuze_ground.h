#ifndef __FUZE_GROUND_H__
#define __FUZE_GROUND_H__

#include "simulation_block_disc.h"

#ifndef NO_REAL_LOCKON
#include "lPointOnSurface.h"
#include "edTerrain/PointOnSurfaceAdapter.h"
#endif

class wFuzeGroundDescriptor;

class wFuzeGround : public wSimulationBlockDisc
{
    DECLARE_BLOCK(wFuzeGround, wSimulationBlockDisc, wFuzeGroundDescriptor);
public:
    wFuzeGround(const wFuzeGroundDescriptor* desc, wSimulationSystem* system);

    virtual double simulate(double time);

protected:
    INPUT_WIRES(3, (
        WIRE(Math::Vec3d, pos_),
        WIRE(Math::Rot3d, rot_),
        WIRE(bool, armed_)
        ));

    OUTPUT_WIRES(0, (
        ));

    OUTPUT_PORT(bool, fire_);

#ifndef NO_REAL_LOCKON
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface surf_;
#else 
	edterrain::PointOnSurfaceAdapter surf_;
#endif
#endif
};

class wFuzeGroundDescriptor : public wSimulationBlockDiscDescriptor
{
    DECLARE_BLOCK_DESCRIPTOR(wFuzeGround)

public:
    virtual wFuzeGround* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
    DECL_PARAM(double, dt);
    DECL_PARAM(double, fire_height);
};

#endif
