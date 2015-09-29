#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "simulation_block_disc.h"

class wColliderBlockDescriptor;

#ifndef NO_REAL_LOCKON
#include "lPointOnSurface.h"
#include "edTerrain/PointOnSurfaceAdapter.h"
#endif

class viObject;
class viSearch;

class wColliderBlock : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wColliderBlock, wSimulationBlock, wColliderBlockDescriptor);
public:
	wColliderBlock(const wColliderBlockDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

protected:
	static bool processIntersection_(viObject *item, viSearch *search, void *data);

protected:
#ifndef NO_REAL_LOCKON
#ifndef USE_TERRAIN4
	landscape::lPointOnSurface point_;		// Collision handling
#else 
	edterrain::PointOnSurfaceAdapter point_;
#endif
#endif
	Math::Vec3d prevPos_;

protected:
	INPUT_WIRES(3, (
		WIRE(Math::Vec3d, position),
		WIRE(bool, checkObjects),
		WIRE(bool, check)
	))

	OUTPUT_WIRES(4, (
		WIRE(Math::Vec3d, collisionPoint),
        // Valid only in object != 0
        WIRE(Math::Vec3d, normal),
		WIRE(int, object),
		WIRE(int, objPartName)
	))

	INPUT_PORT_CONST(int, id_);

	OUTPUT_PORT(bool, collision_);
};

class wColliderBlockDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wColliderBlock);

public:
	virtual wColliderBlock* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, dt);
};

#endif
