#pragma once

#include "Avionics/avLuaDevice.h"

#include "FMElectricSystems/Items/Wire.h"
#include "FMElectricSystems/Items/Source.h"
#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/AutoTransferSwitch.h"
#include "FMElectricSystems/Items/Adapter.h"
#include "Utilities/ccTriggerSystem.h"

namespace cockpit {


class COCKPITBASE_API avElectricSourceParamDriven : public EagleFM::Elec::Source
{
	triggers::double_param_proxy parameter;
public:
	avElectricSourceParamDriven(){}
	void init_with(const char * name)
	{
		parameter.init_with(name);
		update_connection();
	}
	void update_connection()
	{
		setOnOff(parameter > 0);
	}

	bool empty() const { return parameter.empty(); }
};

class COCKPITBASE_API avSimpleElectricSystem : public avLuaDevice
{
private:
	static avSimpleElectricSystem * instance_;
public:
	avSimpleElectricSystem();
	virtual ~avSimpleElectricSystem();

	static avSimpleElectricSystem * get() { return instance_; } ;
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();

	virtual void update();
	//end of interface

	void switchBattery(bool val);
	void switchGenerator(bool val, int num = 1);

	EagleFM::Elec::Wire& getDCbus(int num = 1);
	EagleFM::Elec::Wire& getACbus(int num = 1);

protected:
	void l_register_functions(lua_State *Lua_s,int index);

	static int l_AC_Generator_1_on(lua_State * L);
	static int l_AC_Generator_2_on(lua_State * L);
	static int l_DC_Battery_on(lua_State * L);

	static int l_get_AC_Bus_1_voltage(lua_State * L);
	static int l_get_AC_Bus_2_voltage(lua_State * L);
	static int l_get_DC_Bus_1_voltage(lua_State * L);
	static int l_get_DC_Bus_2_voltage(lua_State * L);
	
	EagleFM::Elec::Source DC_Battery,
		AC_Generator1,
		AC_Generator2;

	EagleFM::Elec::Adapter ACDC_Converter1,
		ACDC_Converter2;

	EagleFM::Elec::Wire DC_Bus1,
		DC_Bus2,
		AC_Bus1,
		AC_Bus2;

	EagleFM::Elec::Switch Sw_Battery,
		Sw_AC_Generator1,
		Sw_AC_Generator2;

	EagleFM::Elec::AutoTransferSwitch DC_Converter1orBattery,
		DC_Converter2orBattery;
};


COCKPITBASE_API avSimpleElectricSystem * getSimpleElectricSystem();


}
