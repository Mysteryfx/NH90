#pragma once
#include "Randomizer/RandomObject.h"
#include "Avionics/avADI.h"
#include "ICommandsCockpitDevice.h"
#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Consumer.h"

using namespace EagleFM;

namespace cockpit { 

typedef enum IKP_clickable_commands
{
	Command_IKP_Pitch_zero_shift = iCommandCockpitDevice_Action_1,
	Command_IKP_testCover,
	Command_IKP_test
};

class COCKPITBASE_API avBaseIKP : public avADI
{
public:
	avBaseIKP();
	virtual ~avBaseIKP();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface

	double get_steering_warn_flag_val() const {return ADI_steering_warning_flag.get_value_out();}
	double get_attitude_warn_flag_val() const {return ADI_attitude_warning_flag.get_value_out();}
	double get_bank_steering() const {return bank_steering.get_value_out();}
	double get_pitch_steering() const {return pitch_steering.get_value_out();}
	double get_airspeed_deviation() const {return airspeed_deviation.get_value_out();}
	double get_track_deviation() const {return track_deviation.get_value_out();}
	double get_height_deviation() const {return height_deviation.get_value_out();}

	virtual void setElecPower(bool flag_in) {ElecSwitchDevicePower.setOnOff(flag_in);}
	virtual bool getElecPower() const {return ElecConDevice.isPowered();}

protected:
	void	   l_register_functions(lua_State *Lua_s,int index);
	static int l_get_steering_warn_flag_val(lua_State *Lua_s);
	static int l_get_attitude_warn_flag_val(lua_State *Lua_s);
	static int l_get_bank_steering(lua_State *Lua_s);
	static int l_get_pitch_steering(lua_State *Lua_s);
	static int l_get_airspeed_deviation(lua_State *Lua_s);
	static int l_get_track_deviation(lua_State *Lua_s);
	static int l_get_height_deviation(lua_State *Lua_s);
protected:
	virtual bool   getGyroPitchBankValid() const;

	virtual double get_pitch_input() const;
	virtual double get_bank_input() const;

	virtual double get_track_deviation_input() const;
	virtual double get_height_deviation_input() const;
	virtual double get_velocity_deviation_input() const;

	virtual Elec::ItemBase * get_connection_bus();
	virtual void   OnPowerOn(const lwl::CSignalInfo* pInfo);
	virtual void   OnPowerOff(const lwl::CSignalInfo* pInfo);

private:
	static const double rnd_scale_factor;

	
	bool SwPosTestCover;

	Elec::Switch	ElecSwitchDevicePower;
	Elec::Consumer	ElecConDevice;


	double pitch_K1_operating,
		pitch_K2_operating,
		pitch_K1_nonoperating,
		pitch_K2_nonoperating;

	gauge_linear ADI_steering_warning_flag;
	gauge_linear ADI_attitude_warning_flag;

	gauge_limited bank_steering;
	gauge_limited pitch_steering;
	gauge_limited airspeed_deviation;
	gauge_limited track_deviation;
	gauge_limited height_deviation;

	bool test_mode;

	double test_pitch, test_bank, test_bank_steering, test_pitch_steering;
	double test_pitch_random_factor, test_bank_random_factor,
		test_bank_steering_random_factor, test_pitch_steering_random_factor;

	double delta_pitch;
};

}
