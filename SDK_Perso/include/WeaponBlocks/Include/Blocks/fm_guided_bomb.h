#ifndef __FM_GUIDED_BOMB_H__
#define __FM_GUIDED_BOMB_H__

#include "fm_bombrocket.h"
#include "Math/FOF.h"

class wFMGuidedBombDescriptor;

class WEAPON_BLOCKS_API wFMGuidedBomb : public wFMBombRocket
{
	DECLARE_BLOCK(wFMGuidedBomb, wFMBombRocket, wFMGuidedBombDescriptor);
public:
	wFMGuidedBomb(const wFMGuidedBombDescriptor* desc, wSimulationSystem* system);

protected:
	virtual void simulateDt(double time, double dt);
protected:
	INPUT_WIRES(2, (
		WIRE(bool, hasSignal_),	//Control signal from autopilot flag
		WIRE(Math::Vec3d, aoa_)	//Commanded AOA vector
	))

	OUTPUT_WIRES(0, (
	))
	
	Math::FOF<Math::Vec3d> aoa[2];
	bool guidedFlight_;					// Скорости не хватает - летим как нурс
};


class WEAPON_BLOCKS_API wFMGuidedBombDescriptor : public wFMBombRocketDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wFMGuidedBomb);

public:
	wFMGuidedBomb* create(Lua::Loader* runtime_cfg,wSimulationSystem* data); 
	void serialize(wBlockSerializer& s);
public:
	
	DECL_PARAM(double[2], dCydA);	//dCy/daoa
	DECL_PARAM(double, A);			//A - polar
	DECL_PARAM(double, maxAoa);		//Maximal angle of attack (rad)
	DECL_PARAM(double, aoaTau);		//Aoa time constant (s)
};


#endif
