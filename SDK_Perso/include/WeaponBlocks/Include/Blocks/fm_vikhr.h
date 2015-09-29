#ifndef __BLOCK_RAMP_H__
#define __BLOCK_RAMP_H__

#include "fm_nurs.h"
#include "../constraint_rail.h"
#include "../attachable.h"

class wFMVikhrDescriptor;

class wFMVikhr : public wFMNurs
{
	DECLARE_BLOCK(wFMVikhr, wFMNurs, wFMVikhrDescriptor);
public:
	wFMVikhr(const wFMVikhrDescriptor* desc, wSimulationSystem* system);

	virtual void simulateDt(double time, double dt);

protected:
	INPUT_WIRES(2, (
		WIRE(bool, hasSignal_),			    // Есть ли наведение?
		WIRE(Math::Vec3d, Nreq_)			// Требуемая перегрузка (в глобальной системе координат)
	))

	OUTPUT_WIRES(0, (
	))

	bool noSpeed_;			// Скорости не хватает - летим как нурс
};

class wFMVikhrDescriptor : public wFMNursDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wFMVikhr)

public:
	virtual void serialize(wBlockSerializer& s);

	virtual wFMVikhr* create(Lua::Loader* runtime_cfg,wSimulationSystem* data);

public:
	DECL_PARAM(double, S);
	DECL_PARAM(double, maxAoa);
};

#endif
