#pragma once
#include "Avionics/avLuaDevice.h"
#include "Avionics/avWeaponsCommon.h"
#include "Avionics\avWeaponsBombSight.h"
#include "Utilities\ccTriggerSystem.h"
namespace cockpit {

class avIRSensor;

enum weapon_sytem_exported_params
{
	WS_GUN_PIPER_AVAILABLE,
	WS_GUN_PIPER_AZIMUTH,
	WS_GUN_PIPER_ELEVATION,
	WS_GUN_PIPER_SPAN,
	WS_TARGET_RANGE,
	WS_TARGET_SPAN,

	WS_ROCKET_PIPER_AVAILABLE,
	WS_ROCKET_PIPER_AZIMUTH,
	WS_ROCKET_PIPER_ELEVATION,

	//DLZ
	WS_DLZ_MIN,
	WS_DLZ_AUTHORIZED,
	WS_DLZ_MAX,
	//

	WS_IR_MISSILE_LOCK,
	WS_IR_MISSILE_TARGET_AZIMUTH,
	WS_IR_MISSILE_TARGET_ELEVATION,
	
	WS_IR_MISSILE_SEEKER_DESIRED_AZIMUTH,
	WS_IR_MISSILE_SEEKER_DESIRED_ELEVATION,

	WS_PARAMS_COUNT,
};

class SimpleWeaponSystemSightUpdate;

class COCKPITBASE_API avSimpleWeaponSystem : public avLuaDevice,
											 public avRippReleaseCapable
{
	friend class SimpleWeaponSystemSightUpdate;

	triggers::double_param_proxy params[WS_PARAMS_COUNT];
public:
	avSimpleWeaponSystem();
	~avSimpleWeaponSystem();
	void initialize(unsigned char ID, const std::string& Name, const std::string& script_name);
	void post_initialize();
	void SetCommand(int command,float value);
	void select_station(int i = -1);
	int  get_weapon_count(int i = -1);
private:
	void l_register_functions(lua_State *Lua_s,int index);	
	void update_ir_seeker();
//////////////////////////////////////////////////////////////////////////
	avIRSensor * IR_missile_seeker;

	void  Launch(bool on_off);
	void  on_pickle(bool on_off);
	int   lua_hook_launch();
	void  launch_station(int i,int substation = -1);
	static int l_select_station(lua_State *Lua_s);
	static int l_launch_station(lua_State *Lua_s);
	static int l_get_weapon_count(lua_State *Lua_s);
	
	static int l_get_chaff_count(lua_State *Lua_s);
	static int l_get_flare_count(lua_State *Lua_s);


	static int l_drop_chaff(lua_State *Lua_s);
	static int l_drop_flare(lua_State *Lua_s);

	static int l_get_ECM_status(lua_State *Lua_s);
	static int l_set_ECM_status(lua_State *Lua_s);

	static int l_set_target_range(lua_State *Lua_s);
	static int l_set_target_span(lua_State *Lua_s);
	static int l_get_target_range(lua_State *Lua_s);
	static int l_get_target_span(lua_State *Lua_s);

	static int l_emergency_jettison(lua_State *Lua_s);
	static int l_emergency_jettison_rack(lua_State *Lua_s);
	static int l_get_station_info(lua_State *Lua_s);

private:
	bool	launch_hook_;
	bool	armed_;

	double  target_range_;
	double  target_span_;

	double  get_target_range() const;
	double  get_target_span() const { return target_span_; }

	
	CannonSight				gun_ballistic;
	wNURSSight				rocket_ballistic;

	MissileSight			missiles_sight;


	piper_omega_lead_data   gun_piper_omega_lead;
	void			update_sight();

	const wShellDescriptor * get_cannon_shell();

	const cPosition  & get_air_target_position();
	const cVector    & get_air_target_velocity();
	const cVector    & get_air_target_N();

	cVector			   get_airspeed();

	SimpleWeaponSystemSightUpdate * sight_timer;
};


class COCKPITBASE_API SimpleWeaponSystemSightUpdate : public avBasicTimer<avSimpleWeaponSystem>
{
	SimpleWeaponSystemSightUpdate();
public:
	SimpleWeaponSystemSightUpdate(avSimpleWeaponSystem *_parent, wModelTime _dtime);
	void NextEvent();
};


}