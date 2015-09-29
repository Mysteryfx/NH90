#ifndef __AFM_GUIDEDBOMB__
#define __AFM_GUIDEDBOMB__

#include "Math/Position.h"
#include "Math/FOF.h"

#include "fm_bombrocket.h"


class wDatabaseLoader;
class wAFMGuidedWeaponDescriptor;

class wAFMGuidedWeaponDescriptor;

// Аэродинамика для бомб и ракет
class WEAPON_BLOCKS_API wAFMGuidedWeapon : public wFMBombRocket
{
	DECLARE_BLOCK(wAFMGuidedWeapon, wFMBombRocket, wAFMGuidedWeaponDescriptor)
public:
	typedef wAFMGuidedWeaponDescriptor ParametersType;

public:
	wAFMGuidedWeapon(const wAFMGuidedWeaponDescriptor* fmData, wSimulationSystem* system);

	// Сделать шаг
	virtual void simulateDt(double time, double dt);

protected:
	virtual void calcForceTorque_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho, Math::Vec3d& R, Math::Vec3d& M);
	virtual Math::Vec3d calcForce_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho);	// Сила (без учета mg)
	virtual Math::Vec3d calcTorque_(const Math::Pos3dd& pos, const Math::Vec3d& v, double sndSpeed, double rho);	// Момент

protected:
	INPUT_WIRES(2, (
		WIRE(bool,			hasSignal_),
		WIRE(Math::Vec3d,	finsReq_)
	))

	OUTPUT_WIRES(3, (
		WIRE(Math::Rot3d,	velRot_),
		WIRE(Math::Rot3d,	realRot_),
		WIRE(Math::Vec3d,	An_)
	))
	Math::Vec3d				aoa_;
	Math::Vec3d				fins_;
};

// Аэродинамические параметры бомб и ракет
class WEAPON_BLOCKS_API wAFMGuidedWeaponDescriptor : public wFMBombRocketDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wAFMGuidedWeapon);

public:
	wAFMGuidedWeapon* wAFMGuidedWeaponDescriptor::create(Lua::Loader* runtime_cfg,wSimulationSystem* data);
	virtual void serialize(wBlockSerializer& s);

public:
	DECL_PARAM(double[2], dCydA);	//dCy/dA (A - aoa in degrees)
	DECL_PARAM(double, A);			//A - polar
	DECL_PARAM(double, maxAoa);		//Maximal Aoa (rad)
	DECL_PARAM(double, finsTau);	//Время полного отклонения рулей (sec)
	DECL_PARAM(double, Sw);			//Площадь крыльев (м^2)
};
#endif //__AFM_GUIDEDBOMB__
