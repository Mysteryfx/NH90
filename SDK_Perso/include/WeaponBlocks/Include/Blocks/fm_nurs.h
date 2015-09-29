#ifndef __BLOCK_ROTATOR_H__
#define __BLOCK_ROTATOR_H__

#include "fm_bombrocket.h"

class wFMNursDescriptor;
class wConstraintRail;

class WEAPON_BLOCKS_API wFMNurs : public wFMBombRocket
{
	DECLARE_BLOCK(wFMNurs, wFMBombRocket, wFMNursDescriptor);
public:
	wFMNurs(const wFMNursDescriptor* desc, wSimulationSystem* system);
	virtual ~wFMNurs();

	virtual void reset(); 
	virtual void simulateDt(double time, double dt);

	virtual void setPosition(const Math::Vec3d& val);
	virtual void setVelocity(const Math::Vec3d& val);
	virtual void setAngularVelocity(const Math::Vec3d& v);
	virtual void setRotation(const Math::Rot3d& val);

protected:
	virtual Math::Vec3d calcTorque_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho);	// Момент
	void	simulateFlight(double time, double dt);

protected:
	INPUT_WIRES(0, ())

	OUTPUT_WIRES(2, (
		WIRE(bool  , wings_out_),
		WIRE(double, rail_passed)
	))

	INPUT_PORT_OBJ_PTR(wConstraintRail, rail_);

	INPUT_PORT_CONST(bool, engine_on_);

	double railX_;
	double railVx_;
	double boostImpulse_;
};


class WEAPON_BLOCKS_API wFMNursDescriptor : public wFMBombRocketDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wFMNurs);

public:
	wFMNurs* create(Lua::Loader* runtime_cfg,wSimulationSystem* data); 

	void setDt(double dt);
	void copyData(const wFMNursDescriptor& desc);

public:
	DECL_PARAM(double, freq);
	DECL_PARAM(bool  , rail_open);
};


#endif
