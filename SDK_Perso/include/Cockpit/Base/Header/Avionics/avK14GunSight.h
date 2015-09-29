#pragma once

#include "Avionics/HUD/avHUD.h"
#include "ICommandsCockpitDevice.h"
#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"
#include "FMElectricSystems/Items/Lamp.h"
#include "FMElectricSystems/Items/AutoTransferSwitch.h"
#include "FMElectricSystems/Items/MultiThrowSwitch.h"
#include "FMElectricSystems/Items/VoltageRegulator.h"

namespace cockpit {

class SimpleGyroSight;

class COCKPITBASE_API avK14GunSight : public avHUD
{

protected:

	enum K14SightFailures
	{
		K14_NO_FAILURE  = -1,
		K14_NO_POWER_SUPPLY = 0,
		K14_FIXED_LAMP_DEFECTIVE,
		K14_MOV_LAMP_DEFECTIVE,
		K14_MOTOR_DEFECTIVE
	};

public:

	enum K14_mode
	{
		K14_OFF				= 0,
		K14_ON				= 1,
	};

	enum Sight_Mode 
	{
		FIXED_SIGHT = 0,
		FIXED_GYRO_SIGHT,
		GYRO_SIGHT
	};

	enum K14_Commands
	{
		SPAN_SCALE_ROTATING = iCommandCockpitDevice_Action_1,
		RANGE_GRIP_ROTATING,
		CHANGE_SIGHT_MODE,
		GYRO_POWER,
		SIGHT_BRIGHTNESS,
		RING_BLANC_LEVER,
		SUN_SHADE_LEVER,

		GYRO_POWER_EXTERNAL,
		RING_BLANC_LEVER_EXTERNAL,
		SIGHT_BRIGHTNESS_EXTERNAL,

		RANGE_AXIS,
		SPAN_AXIS,
		SIGHT_BRIGHTNESS_AXIS,
		SPAN_SCALE_ROTATING_EXT,

		GYRO_POWER_STATIC,
		CHANGE_SIGHT_MODE_STATIC,
		CHANGE_SIGHT_MODE_ITER,
		K14_COMMAND_MAX
	};

	avK14GunSight();
	virtual ~avK14GunSight();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void release();
	virtual void SetCommand(int command, float value /* = 0 */);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

	virtual void set_mode(const Device_Mode& new_mode);
	const Device_Mode& get_mode() const {return mode_;}
	void connect_electric_wire(EagleFM::Elec::Wire& power_wire);

	virtual bool getElecPower() const {return gyro_power.isPowered();}

	bool get_ring_blanc_lever() const {return ring_blanc_lever_;} 

	virtual float get_range() const {return current_range_;}
	float getRangeArg() const {return range_lever_;}

	float getSpanMaskAngle() const 
	{
		return spanMaskAngle;
	}
	
	float getRangeMaskAngle() const 
	{
		return limitedRangeMaskAngle;
	}

	const cVector& get_gyro_sight_point() const
	{
		return gyro_sight_point_;
	}

	bool get_fixed_reticle_visible() const
	{
		return fixed_bulb_consumer_.isPowered();
	}

	bool get_movable_reticle_visible() const
	{
		return movable_bulb_consumer_.isPowered();
	}

	double getFixedReticleLightness() const;
	double getMovableReticleLightness() const;

	double getDbgSightSize() const;

	static const Device_Mode MODE_OFF;
	static const Device_Mode MODE_ON;

protected:
	void onPowerOff(const lwl::CSignalInfo* pInfo);

	class GunsSightUpdate_timer : public avDevice_BasicTimer
	{
	public:
		GunsSightUpdate_timer(avDevice* p, wModelTime _dtime) : avDevice_BasicTimer(p, _dtime){};
		virtual ~GunsSightUpdate_timer() {}
		void NextEvent();
	};

	void	read_coefficients(Lua::Config& config);

	void    limitRangeMaskAngle();
	void	calculateRangeMaskAngle(double leverPos);
	void	calculateSpanMaskAngle(double leverPos);

	void	update_gyro_sight();

	Device_Mode mode_;

	float	current_range_, current_span_; // in feet

	float	range_lever_;
	float	base_lever_;

	float	rangeMaskAngle, limitedRangeMaskAngle;
	float	spanMaskAngle;

	// Argument to the mask rotation angle ratios
	// span
	double K_ang_L;
	// distance
	double K_ang_D;
	
	double Dmin, Dmax, Dmin2;
	double Lmin, Lmax;
	double kD, bD;
	double kL, bL;

	double relAngleMax;

	SimpleGyroSight* gun_sight_;
	cVector omega;

	cVector	gyro_sight_point_;

	GunsSightUpdate_timer*  pGyro_sight_timer_;

	EagleFM::Elec::VoltageRegulator voltageRegulator;
	EagleFM::Elec::Lamp     fixed_bulb_consumer_;
	EagleFM::Elec::Lamp     movable_bulb_consumer_;
	EagleFM::Elec::AutoTransferSwitch fixed_bkps_;
	EagleFM::Elec::AutoTransferSwitch movable_bkps_;
	EagleFM::Elec::MultiThrowSwitch mode_switch_;
	EagleFM::Elec::Wire		common_wire_;
	EagleFM::Elec::Wire		power_wire_;

	EagleFM::Elec::Switch	gyro_power_switch_;
	EagleFM::Elec::Consumer gyro_power;

	bool	ring_blanc_lever_;
};

}
