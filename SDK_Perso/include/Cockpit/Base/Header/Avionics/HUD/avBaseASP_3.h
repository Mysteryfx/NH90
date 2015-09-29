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

class COCKPITBASE_API avBaseASP_3 : public avHUD
{

protected:

	enum ASP_3NSightFailures
	{
		ASP_3N_NO_FAILURE  = -1,
		ASP_3N_NO_POWER_SUPPLY = 0,
		ASP_3N_LAMP_DEFECTIVE,
		ASP_3N_MOTOR_DEFECTIVE
	};

public:

	enum ASP_3N_mode
	{
		ASP_3N_OFF				= 0,
		ASP_3N_ON				= 1,
	};

	enum ASP_3N_Commands
	{
		SPAN_SCALE_ROTATING = iCommandCockpitDevice_Action_1,
		RANGE_GRIP_ROTATING,
		CHANGE_SIGHT_MODE,
		SIGHT_BRIGHTNESS,
		SIGHT_BRIGHTNESS_EXT,

		RANGE_AXIS,
		SPAN_AXIS,
		SIGHT_BRIGHTNESS_AXIS,
		SPAN_SCALE_ROTATING_EXT,
		RANGE_ROTATING_EXT,
		SPARE_SIGHT,

		COLOUR_FILTER,

		// input commands
		SPARE_SIGHT_EXT,
		COLOUR_FILTER_EXT,
		CHANGE_SIGHT_MODE_EXT,


		REFLECTOR_MIRROR_ROTATE,
		REFLECTOR_MIRROR_ROTATE_EXT,
		ASP_3N_COMMAND_MAX
	};

	avBaseASP_3();
	virtual ~avBaseASP_3();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void release();
	virtual void SetCommand(int command, float value /* = 0 */);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	virtual void external_parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);
	//

	//avBreakable interface
	virtual bool set_damage(unsigned Failure_ID, const bool can_be_rejected /* = false */);
	virtual void repair();
	//end of interface

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
		return rangeMaskAngle;
	}

	const cVector& get_gyro_sight_point() const
	{
		return gyro_sight_point_;
	}

	bool get_movable_reticle_visible() const
	{
		return bulb_consumer_.isPowered();
	}

	bool get_movable_reticle_on() const
	{
		return mode_switch_.getIsOn();
	}

	double getMovableReticleLightness() const;

	double getDbgSightSize() const;

	double	getCurrentSpan()	const { return current_span_; }
	float 	getMirrorRotation()	const { return mirrorRotation_; }

protected:


	double get_teta_for_radius(double mils) const
	{
		return (1/B) * log(mils/R0);
	}

	double get_radius_for_teta(double teta) const
	{
		return R0 * exp(B * teta);
	}

	virtual void connect_electric() {} ;

	void onPowerOff(const lwl::CSignalInfo* pInfo);

	class GunsSightUpdate_timer : public avDevice_BasicTimer
	{
	public:
		GunsSightUpdate_timer(avDevice* p, wModelTime _dtime) : avDevice_BasicTimer(p, _dtime){};
		virtual ~GunsSightUpdate_timer() {}
		void NextEvent();
	};

	void	read_coefficients(Lua::Config& config);

	void    calculateMasksAngles();

	void	update_gyro_sight();

	float	current_range_, current_span_;

	float	range_lever_;
	float	base_lever_;

	float	rangeMaskAngle;
	float	spanMaskAngle;

	double Dmin, Dmax;
	double Lmin, Lmax;
	double kD, bD;
	double kL, bL;

	float R0;
	float Rmax;
	float B;

	float   mirrorRotation_;

	SimpleGyroSight* gun_sight_;
	cVector omega;

	cVector	gyro_sight_point_;

	GunsSightUpdate_timer*  pGyro_sight_timer_;

	EagleFM::Elec::VoltageRegulator voltageRegulator;
	EagleFM::Elec::Lamp     bulb_consumer_;
	EagleFM::Elec::Consumer gyro_consumer_;
	EagleFM::Elec::Switch	mode_switch_;
	EagleFM::Elec::Wire		power_wire_;

	EagleFM::Elec::Consumer gyro_power;

	bool	ring_blanc_lever_;

	ed::vector<clickableElementData*> cl_pos_upd_elemnts_;
};

}
