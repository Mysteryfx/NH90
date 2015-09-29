#ifndef AV_A_RV_ALTIMETER_H
#define AV_A_RV_ALTIMETER_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

/************************************************************************/
/* A-xx, RV-xx  Radar altimeter indicator                               */
/************************************************************************/

#include "Avionics/avRadarAltimeterBase.h"
#include "Avionics/avMechanics.h"

#include "ICommandsCockpitDevice.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Lamp.h"

using namespace EagleFM;

namespace cockpit {

class COCKPITBASE_API avA_RV_Altimeter : public avRadarAltimeterBase
{
protected:
	typedef enum RALT_clickable_commands
	{
		CMD_RALT_ROTARY = iCommandCockpitDevice_Action_1,
		CMD_RALT_TEST,
		CMD_RALT_MAX
	};

public:
	avA_RV_Altimeter();
	virtual ~avA_RV_Altimeter();
	
	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer){};
	virtual void update();
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface	

	//avBasicElectic interface
	virtual void setElecPower(bool flag_in) {ElecSwitchDevicePower.setOnOff(flag_in);}

	// Linear output. Use it if you need the current height
	// as it is shown by the cockpit indicator.
	double getIndicatorAltitude() const;
	double getRALT_warning_flag() const {return RALT_warning_flag.get_value_out();}
    bool getLamp_DangerousHeightStatus() const { return ElecCon_LampDangerousHeight.getLightness(); }
	double getDangerousHeight() const {return dangerousHeight;}
	bool getDangerousHeightFlag() const {return ISdangerousHeight;}
	bool getISdangerousHeightSound() const {return ISdangerousHeightSound;}

	void clear() {ISdangerousHeight = false;}

protected:
	void	   l_register_functions(lua_State *Lua_s,int index);
	static int l_get_needle_value(lua_State *Lua_s);
	static int l_get_warning_flag(lua_State *Lua_s);
	static int l_get_dangerous_height(lua_State *Lua_s);
	static int l_get_dangerous_height_flag(lua_State *Lua_s);

	void		connect_electric(EagleFM::Elec::ItemBase& AC_wire, EagleFM::Elec::ItemBase& DC_wire);

protected:

	Elec::Switch	ElecSwitchDevicePower;

	Elec::Switch	ElecSwitch_LampDangerousHeight;
	Elec::Lamp		ElecCon_LampDangerousHeight;

	void   OnPowerOn(const lwl::CSignalInfo* pInfo);
	void   OnPowerOff(const lwl::CSignalInfo* pInfo);

	bool RALT_warning;
	bool IsInTest;

	gauge_limited	RALT_needle;
	gauge_linear	RALT_warning_flag;

	double test_RALT;
	double dangerousHeight;

	bool ISdangerousHeight, PrevISdangerousHeight;
	bool ISdangerousHeightSound;
	wModelTime ISdangerousHeightSoundTime;
	double PrevAltitude;

	bool warmed_up_;
};
} 

#endif // AV_A_RV_ALTIMETER_H
