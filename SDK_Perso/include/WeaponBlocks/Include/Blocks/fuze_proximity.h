#ifndef __FUZE_PROXIMITY_H__
#define __FUZE_PROXIMITY_H__

#include "simulation_block_disc.h"

#ifndef NO_REAL_LOCKON
#include "viObject.h"
#include "viSearch.h"
#endif

class wFuzeProximityDescriptor;

class wFuzeProximity : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wFuzeProximity, wSimulationBlockDisc, wFuzeProximityDescriptor);
public:
	wFuzeProximity(const wFuzeProximityDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

protected:
#ifndef NO_REAL_LOCKON
	void testCollision_(); 

	bool processObjects_(viObject *item, viSearch *search, void *data);
	static bool processObjectsS_(viObject *item, viSearch *search, void *data);

	void scheduleExplosion_(double dt);
#endif

protected:
	INPUT_WIRES(3, (
		WIRE(Math::Vec3d, pos_),
		WIRE(bool, armed_),
		WIRE(bool, delay_)
	));

	OUTPUT_WIRES(1, (
		WIRE(Math::Vec3d, explosionPoint_)
	));

	INPUT_PORT_CONST(int, id_);

	OUTPUT_PORT(bool, explode_);

	Math::Vec3d prevPos_;
	double explosionTime_;
};

class wFuzeProximityDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wFuzeProximity)

public:
	virtual wFuzeProximity* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, dt);

	DECL_PARAM(double, radius);
	DECL_PARAM(double, delay_small);
	DECL_PARAM(double, delay_large);

	DECL_PARAM(double, arm_delay);
};

#endif
