#include "StdAfx.h"
#include "Avionics/avSimpleWeaponSystem.h"
#include "ccModuleManager.h"
#include "iCommand.h"
#include "IwHumanPLane.h"
#include "IwHumanPayload.h"
#include "Shells/shell_descriptors.h"
#include "Guns\aircraft_gun_mount.h"
#include "Guns\ammo_supply.h"
#include "Utilities/avDynamicBodyProxy.h"
#include "ammunition_nurs.h"
#include "Avionics/Sensors/avLinkToTargetResponder.h"
#include "IwoAIPilon.h"
#include "Avionics/Sensors/IRMissiles/avSidewinderSeeker.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avSimpleWeaponSystem, avSimpleWeaponSystem);

#define DECLARE_EXPORTED_PARAMETER(name,def_value) params[name].init_with(#name); params[name] = def_value;

avSimpleWeaponSystem::avSimpleWeaponSystem():
launch_hook_(false),
armed_(true),
target_range_(1000.0),
target_span_(15.0),
IR_missile_seeker(nullptr)
{
	DECLARE_EXPORTED_PARAMETER(WS_GUN_PIPER_AVAILABLE  ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_GUN_PIPER_AZIMUTH    ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_GUN_PIPER_ELEVATION  ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_GUN_PIPER_SPAN	   ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_TARGET_RANGE		   ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_TARGET_SPAN		   ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_ROCKET_PIPER_AVAILABLE,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_ROCKET_PIPER_AZIMUTH  ,0.0);
	DECLARE_EXPORTED_PARAMETER(WS_ROCKET_PIPER_ELEVATION,0.0);

	DECLARE_EXPORTED_PARAMETER(WS_DLZ_MIN,-1.0);
	DECLARE_EXPORTED_PARAMETER(WS_DLZ_MAX,-1.0); 

	DECLARE_EXPORTED_PARAMETER(WS_IR_MISSILE_LOCK,0); 
	DECLARE_EXPORTED_PARAMETER(WS_IR_MISSILE_TARGET_AZIMUTH  ,0); 
	DECLARE_EXPORTED_PARAMETER(WS_IR_MISSILE_TARGET_ELEVATION,0); 
	DECLARE_EXPORTED_PARAMETER(WS_IR_MISSILE_SEEKER_DESIRED_AZIMUTH,0); 
	DECLARE_EXPORTED_PARAMETER(WS_IR_MISSILE_SEEKER_DESIRED_ELEVATION,0); 

	//setted up automatically by calling set_timer
	avWeap_ReleaseTimer_Activity * timer = new avWeap_ReleaseTimer_Activity(this);
	sight_timer							 = new SimpleWeaponSystemSightUpdate(this,0.02);
}

template<class V3>
Math::Vec3d toVec3d(const V3 & v)
{
	return Math::Vec3d(v.x,v.y,v.z);
}

template<class V3>
cVector to_cVector(const V3 & v)
{
	return cVector(v.x,v.y,v.z);
}

avSimpleWeaponSystem::~avSimpleWeaponSystem()
{
	if (sight_timer)
	{
		sight_timer->SetParent(0);
		sight_timer->KillMe();
		sight_timer = 0;
	}
	if (release_timer)
	{
		release_timer->Die();
		release_timer = nullptr;
	}

	if (IR_missile_seeker)
		delete IR_missile_seeker;
}


void avSimpleWeaponSystem::initialize( unsigned char ID, const std::string& Name, const std::string& script_name )
{
	avLuaDevice::initialize(ID,Name,script_name);
	listen_command(this,iCommandPlaneFire);
	listen_command(this,iCommandPlaneFireOff);
	listen_command(this,iCommandPlanePickleOn);
	listen_command(this,iCommandPlanePickleOff);
	listen_command(this,iCommandPlaneChangeWeapon);
	listen_command(this,iCommandSwitchMasterArm);

	listen_command(this,iCommandPlaneIncreaseBase_Distance); 
	listen_command(this,iCommandPlaneDecreaseBase_Distance) ;
	listen_command(this,iCommandPlaneStopBase_Distance);
	listen_command(this,iCommandPlaneJettisonFuelTanks);
	listen_command(this,iCommandPlaneJettisonFuelTanksUp);
	listen_command(this,iCommandPlaneJettisonWeapons);
	listen_command(this,iCommandPlaneJettisonWeaponsUp);

	lua_State * Ls = ensured_Lua("avSimpleWeaponSystem");
	if (Ls)
	{
		lua_getglobal(Ls,"on_launch");
		launch_hook_ = lua_isfunction(Ls,-1);
		lua_pop(L,1);
	}
}

void avSimpleWeaponSystem::post_initialize()
{
	avLuaDevice::post_initialize();
	sight_timer->Start(0);
}

void avSimpleWeaponSystem::SetCommand(int command,float value)
{
	switch (command)
	{
	case iCommandPlaneIncreaseBase_Distance:
		target_span_ += 0.5;
		if (target_span_ > 80)
			target_span_ = 80;
		break;
	case iCommandPlaneDecreaseBase_Distance:
		target_span_ -= 0.5;
		if (target_span_ < 8)
			target_span_ = 8;
		break;
	case iCommandPlaneStopBase_Distance:
		break;
	case iCommandSwitchMasterArm:
		armed_ = !armed_;
		break;
	case iCommandPlaneFire:
		if (armed_)
			avWeaponsCommon::fire_cannon(true);
		break;
	case iCommandPlaneFireOff:
		avWeaponsCommon::fire_cannon(false);
		break;
	case iCommandPlanePickleOn:
		if (armed_)
			on_pickle(true);
		break;
	case iCommandPlanePickleOff:
		on_pickle(false);
		break;
	case iCommandPlaneChangeWeapon:
		{
			IwHumanPayload * load_out = get_context()->human ? get_context()->human->getPayloadInterface() : NULL;
			if (!load_out)
				return;
			load_out->NextWeapon();
		}
		break;
	default:
		{
			avLuaDevice::SetCommand(command,value);
			break;
		}
	}
}

void avSimpleWeaponSystem::Launch(bool on_off)
{
	IwHumanPayload * load_out = get_context()->human ? get_context()->human->getPayloadInterface() : NULL;
	if (!load_out)
		return;
	if (on_off)
	{
		if (armed_)
		{
			int hook_res = lua_hook_launch();
			launch_station(load_out->get_current_pilon());
		}
	}
	else
	{
		for (int i = 0 ; i < load_out->get_pilon_count() ; ++i)
			get_context()->human->getInterfaceToWoLA()->stop_gun_fire(i);
	}
}

void avSimpleWeaponSystem::on_pickle(bool on_off)
{
	if (release_timer)
	{
		if (on_off)
			release_timer->start_unloading(0.5);
		else
			release_timer->stop_unloading();
	}
}

void avSimpleWeaponSystem::select_station(int i)
{
	if (get_context()->human &&
		get_context()->human->getPayloadInterface())
	{
		get_context()->human->getPayloadInterface()->set_active_station(i);
	}
}


int avSimpleWeaponSystem::get_weapon_count( int i /*= -1*/ )
{
	if (get_context()->human &&
		get_context()->human->getPayloadInterface())
	{
		return get_context()->human->getPayloadInterface()->get_current_weapon_count_on_pilon(i);
	}
	return 0;
}



#define L_BIND(fname) 	lua_pushstring(Lua_s,#fname);	lua_pushcfunction(Lua_s,l_##fname);	lua_settable  (Lua_s,index)


int avSimpleWeaponSystem::l_select_station(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	system->select_station(lua_isnumber(Lua_s,2) ? static_cast<int>(lua_tonumber(Lua_s,2)): -1);
	return 0;
}

int avSimpleWeaponSystem::l_launch_station(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	system->launch_station(lua_isnumber(Lua_s,2) ? static_cast<int>(lua_tonumber(Lua_s,2)): -1);
	return 0;
}


int  avSimpleWeaponSystem::l_get_weapon_count(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	lua_pushnumber(Lua_s,system->get_weapon_count(lua_isnumber(Lua_s,2) ? static_cast<int>(lua_tonumber(Lua_s,2)): -1));
	return 1;
}


int   avSimpleWeaponSystem::l_get_target_range(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	lua_pushnumber(Lua_s,system->get_target_range());
	return 1;
}

int   avSimpleWeaponSystem::l_set_target_range(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	if (lua_isnumber(Lua_s,2))
		system->target_range_ = lua_tonumber(Lua_s,2);
	return 0;
}

int   avSimpleWeaponSystem::l_get_target_span(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	lua_pushnumber(Lua_s,system->get_target_span());
	return 1;
}

int   avSimpleWeaponSystem::l_set_target_span(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	if (lua_isnumber(Lua_s,2))
		system->target_span_ = lua_tonumber(Lua_s,2);
	return 0;
}


int avSimpleWeaponSystem::l_get_chaff_count(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	IwHumanPayload		*  load_out = system->get_context() ? system->get_context()->human->getPayloadInterface() : NULL;
	int group = -1;
	if (lua_isnumber(Lua_s,2))
	{
		group = lua_tonumber(Lua_s,2);
	}
	lua_pushnumber(Lua_s,load_out->get_nChaff(group));
	return 1;
}

int avSimpleWeaponSystem::l_get_flare_count(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	IwHumanPayload		*  load_out = system->get_context() ? system->get_context()->human->getPayloadInterface() : NULL;
	int group = -1;
	if (lua_isnumber(Lua_s,2))
	{
		group = lua_tonumber(Lua_s,2);
	}
	lua_pushnumber(Lua_s,load_out->get_nFlare(group));
	return 1;
}


int avSimpleWeaponSystem::l_get_ECM_status(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	lua_pushboolean(Lua_s,system->get_context()->human_ptr->IsFlag(wsFlag_Disturbance));
	return 1;
}

int avSimpleWeaponSystem::l_set_ECM_status(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	bool value = false;
	if (lua_isboolean(Lua_s,2))
	{
		value = lua_toboolean(Lua_s,2);
	}
	if (value)
		system->get_context()->human_ptr->SetFlag(wsFlag_Disturbance);
	else
		system->get_context()->human_ptr->ResetFlag(wsFlag_Disturbance);
	return 0;

}

int avSimpleWeaponSystem::l_drop_chaff(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	int count = 1;
	if (lua_isnumber(Lua_s,2))
		count = lua_tonumber(Lua_s,2);
	int group = -1;
	if (lua_isnumber(Lua_s,3))
		group = lua_tonumber(Lua_s,3);
	system->get_context()->human->Create_Chaff_Human(count,group);
	return 0;
}

int avSimpleWeaponSystem::l_drop_flare(lua_State *Lua_s)
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	int count = 1;
	if (lua_isnumber(Lua_s,2))
		count = lua_tonumber(Lua_s,2);
	int group = -1;
	if (lua_isnumber(Lua_s,3))
		group = lua_tonumber(Lua_s,3);
	system->get_context()->human->Create_Flare_Human(count,group);
	return 0;
}

void avSimpleWeaponSystem::l_register_functions( lua_State *Lua_s,int index )
{
	avLuaDevice::l_register_functions(Lua_s,index);
	L_BIND(select_station);
	L_BIND(launch_station);
	L_BIND(set_target_span);
	L_BIND(get_target_span);
	L_BIND(set_target_range);
	L_BIND(get_target_range);
	L_BIND(emergency_jettison);
	L_BIND(emergency_jettison_rack);
	L_BIND(get_station_info);
	L_BIND(get_flare_count);
	L_BIND(get_chaff_count);
	L_BIND(drop_flare);
	L_BIND(drop_chaff);
	L_BIND(get_ECM_status);
	L_BIND(set_ECM_status);
}

int avSimpleWeaponSystem::lua_hook_launch()
{
	if (launch_hook_)
	{
		int result = 0;
		lua_State * Ls = ensured_Lua();
		lua_getglobal(Ls,"on_launch");
		if (lua_isfunction(Ls,-1))
		{
			if (lua_pcall(Ls, 0, 1, 0) != 0)
			{
				ED_ERROR("avSimpleWeaponSystem Lua \"on_launch\"  : %s", lua_tostring(Ls,-1));
				lua_pop(Ls, 1);
			}
			else
			{
				result = lua_tonumber(Ls,-1);
			}
		}
		lua_pop(L,1);
		return result;
	}
	return 0;
}

void avSimpleWeaponSystem::launch_station( int i,int substation)
{
	if (!armed_)
	{
		if (release_timer)
			release_timer->stop_unloading();
		return;
	}

	IwHumanPayload * load_out = get_context()->human ? get_context()->human->getPayloadInterface() : NULL;

	if (load_out)
	{
		const wsType & weapon	   = load_out->ContainerType(i);
		ObjectID target			   = 0;
		const cPosition & pos	   = get_context()->human_ptr->Position(); 
		cVector  target_point	   = pos.p + pos.x * 10000.0;
		 		 target_point.y   += 2000.0f;

		bool single_launch_ = false;
		if (load_out->isRadarGuided(i))
		{
			if (get_context()->link_to_target)
				target = get_context()->link_to_target->get_target_to_engage();
			single_launch_ = true;
		}
		else if (load_out->isInfraredSeeker(i))
		{
			if (IR_missile_seeker &&
				IR_missile_seeker->getTarget().world_id)
			{
				target			= IR_missile_seeker->getTarget().world_id;
				target_point    = IR_missile_seeker->getTarget().get_point();
				IR_missile_seeker->clear();
			}
			single_launch_ = true;
		}
		else if (load_out->isLaserHoming(i))
		{
			single_launch_ = true;
		}
		else if (load_out->isTVhoming(i))
		{
			single_launch_ = true;
		}
		else if (load_out->isGunPod(i))
		{
			get_context()->human->getInterfaceToWoLA()->start_gun_fire(i,substation);
			return;
		}
		
		int count_before_launch = load_out->get_current_weapon_count_on_pilon();
		unsigned int		 id = load_out->LaunchStation(i,substation,target,target_point,armed_);
		if (release_timer && release_timer->IsActive())
		{
			if (id && count_before_launch == 1)
			{
				load_out->NextWeapon();
				if (release_timer)
					release_timer->stop_unloading();
			}
			if (single_launch_)
			{
				if (release_timer)
					release_timer->stop_unloading();
			}			
		}
	}
}

void avSimpleWeaponSystem::update_sight()
{
	double range = get_target_range();
	params[WS_TARGET_RANGE] = range;
	params[WS_TARGET_SPAN]  = get_target_span();

	double piper_span = get_target_span();
	if (range > 1.0)
		piper_span /= range;
	
	params[WS_GUN_PIPER_SPAN]  = piper_span;

	dVector curr_omega(get_context()->human_ptr->Omx(),
					   get_context()->human_ptr->Omy(),
					   get_context()->human_ptr->Omz());	
	double  dt		    = sight_timer->GetDeltaTime();
	dVector velocity	= get_context()->human_ptr->VectorVelocity();
	dVector wind(0.0,0.0,0.0);
	cPosition human_pos = get_context()->human_ptr->Position();

	if (const wShellDescriptor *shell = get_cannon_shell())
	{
		gun_ballistic.loadShell(*shell);
		double			 TOF	  = 0;
		cPosition muzzle_position = get_context()->human->getInterfaceToWoLA()->get_gun_position();
		dVector impact_point;
		gun_ballistic.sightDist(muzzle_position.p.y, 
								range,
								velocity + muzzle_position.x * shell->v0, 
								dVector(0.0,0.0,0.0),
								impact_point,
								TOF);


		impact_point.x += muzzle_position.p.x;
		impact_point.z += muzzle_position.p.z;

		gun_piper_omega_lead.update(curr_omega,dt,TOF);

		impact_point -= get_context()->human_ptr->GetPoint();
		human_pos.p	  = cVector();
		impact_point  = to_local_cs(human_pos,impact_point);
		impact_point = impact_point.rotY(gun_piper_omega_lead.piper_rotation.y);
		impact_point = impact_point.rotZ(gun_piper_omega_lead.piper_rotation.z);
		impact_point = impact_point.rotX(gun_piper_omega_lead.piper_rotation.x);

		PolarPosition p(impact_point);

		params[WS_GUN_PIPER_AVAILABLE] = 1.0;
		params[WS_GUN_PIPER_AZIMUTH]   = p.azimuth;
		params[WS_GUN_PIPER_ELEVATION] = p.elevation;
	}
	else
	{
		params[WS_GUN_PIPER_AVAILABLE] = 0.0;
		params[WS_GUN_PIPER_AZIMUTH]   = 0.0;
		params[WS_GUN_PIPER_ELEVATION] = 0.0;
	}

	IwHumanPayload * load_out = get_context()->human ? get_context()->human->getPayloadInterface() : NULL;

	if (!load_out)
	{
		params[WS_ROCKET_PIPER_AVAILABLE] = 0.0;
		params[WS_ROCKET_PIPER_AZIMUTH]   = 0.0;
		params[WS_ROCKET_PIPER_ELEVATION] = 0.0;
		return;
	}

	if (load_out->isUnguidedRocket())
	{
		wsType type_ = load_out->ContainerType();
		if (rocket_ballistic.getCurrent_wsType() != type_)
		{
			rocket_ballistic.setWsType(type_);		
			wAmmunitionNURSDescriptor* desc = wGetAmmunitionNURSByType(type_);
			if (desc)
				setNURS_SightByDesc(&rocket_ballistic, desc);
		}

		cPosition pos =  get_context()->human->getInterfaceToWoLA()->get_pilon_position(load_out->get_current_pilon());
		Math::Vec3d ip;
		double		TOF;
		Math::Matrix3d m;
		m.x().set(pos.x.x, pos.x.y, pos.x.z);
		m.y().set(pos.y.x, pos.y.y, pos.y.z);
		m.z().set(pos.z.x, pos.z.y, pos.z.z);
		rocket_ballistic.calculateImpactPoint(pos.p.y,
												Math::Rot3d(m), 
												Math::Vec3d(velocity.x,velocity.y,velocity.z), 
												wNURSSight::BY_DIST,
											    range, 
												2,
												ip,
												TOF);
		dVector impact_point;

		impact_point.x = pos.p.x + ip.x;
		impact_point.y = ip.y;
		impact_point.z = pos.p.z + ip.z;
		impact_point  += wind*TOF;

		impact_point = to_local_cs(human_pos,impact_point);
		PolarPosition p(impact_point);

		params[WS_ROCKET_PIPER_AVAILABLE] = 1.0;
		params[WS_ROCKET_PIPER_AZIMUTH]   = p.azimuth;
		params[WS_ROCKET_PIPER_ELEVATION] = p.elevation;
	}
	if (load_out->isAirToAir())
	{
		missiles_sight.setMissileData(load_out->ContainerType());
		missiles_sight.set_external_distance(range);
		missiles_sight.calculate();

		params[WS_DLZ_MIN]			=	missiles_sight.get_DLZ().dMIN;
		params[WS_DLZ_AUTHORIZED]	=	missiles_sight.get_DLZ().dAUT;
		params[WS_DLZ_MAX]			=	missiles_sight.get_DLZ().dMAX;



		if (load_out->isInfraredSeeker())
		{

		}
	}
	else
	{
		params[WS_DLZ_MIN]			=	-1;
		params[WS_DLZ_AUTHORIZED]	=	-1;
		params[WS_DLZ_MAX]			=	-1;
	}

	update_ir_seeker();

}

const wShellDescriptor * avSimpleWeaponSystem::get_cannon_shell()
{
	wAircraftGunMount * mount = get_context()->human->getPayloadInterface()->getDefaultGunMount();
	if (mount)
		return mount->getAmmoSupply()->getCurrentShell();
	return 0;
}

const cPosition  & avSimpleWeaponSystem::get_air_target_position()
{
	//FIXME
	static cPosition p;
	p    = get_context()->human_ptr->Position();
	p.p += get_airspeed() * 10.0;
	return p;
}
const cVector    & avSimpleWeaponSystem::get_air_target_velocity()
{
	//FIXME
	static cVector v;
	return v = get_airspeed();
	return v;
}

const cVector    & avSimpleWeaponSystem::get_air_target_N()
{
	//FIXME
	static cVector v;
	v.x = get_context()->human_ptr->Nx();
	v.y = get_context()->human_ptr->Ny();
	v.z = get_context()->human_ptr->Nz();
	return v;
}

cVector avSimpleWeaponSystem::get_airspeed()
{
	return get_context()->human_ptr->VectorVelocity();
}


SimpleWeaponSystemSightUpdate::SimpleWeaponSystemSightUpdate( avSimpleWeaponSystem *_parent, wModelTime _dtime ):
avBasicTimer<avSimpleWeaponSystem>(_parent,_dtime)
{

}

void SimpleWeaponSystemSightUpdate::NextEvent()
{
	if (parent)
	{
		parent->update_sight();
		wakeUpTime = wTime::GetModelTime() + dtime;
	}
}

double cockpit::avSimpleWeaponSystem::get_target_range() const
{
	return target_range_;
}

int cockpit::avSimpleWeaponSystem::l_emergency_jettison( lua_State *Lua_s )
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	if (system)
	{
		int pilon = -1;
		int count =  0;
		if (lua_isnumber(Lua_s,2))
			pilon = lua_tonumber(Lua_s,2);
		if (lua_isnumber(Lua_s,3))
			count = lua_tonumber(Lua_s,3);
		IwHumanPayload * load_out = system->get_context()->human->getPayloadInterface();
		if (load_out)
		{
			if (pilon < 0)
				load_out->emergency_jettison();
			else
				load_out->emergency_jettison(pilon,count);
		}
	}
	return 0;
}

int cockpit::avSimpleWeaponSystem::l_emergency_jettison_rack( lua_State *Lua_s )
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	if (system)
	{
		int pilon = -1;
		if (lua_isnumber(Lua_s,2))
			pilon = lua_tonumber(Lua_s,2);
		IwHumanPayload * load_out = system->get_context()->human->getPayloadInterface();
		if (load_out && pilon > -1)
			load_out->emergency_jettison_rack(pilon);
	}
	return 0;
}

int avSimpleWeaponSystem::l_get_station_info( lua_State *Lua_s )
{
	avSimpleWeaponSystem * system = avSimpleWeaponSystem::l_get_device_from_meta<avSimpleWeaponSystem>(Lua_s);
	if (system)
	{
		unsigned station_number = 0;
		if (lua_isnumber(Lua_s,2))
			station_number = lua_tonumber(Lua_s,2);
		const IwoAIPilon * station = system->get_context()->human->getInterfaceToWoLA()->get_weapon_station(station_number);
		if (!station)
			return 0;
		station->push_to_state(Lua_s);
		return 1;
	}
	return 0;
}

void avSimpleWeaponSystem::update_ir_seeker()
{
	IwHumanPayload * load_out = get_context()->human->getPayloadInterface();
	if (load_out && load_out->isInfraredSeeker())
	{
		if (!IR_missile_seeker)
		{
			avSidewinderSeeker * seeker =  new avSidewinderSeeker();
			IR_missile_seeker = seeker;
			IR_missile_seeker->set_carrier(get_context()->human_ptr);
			IR_missile_seeker->initialize();
		}
		wsType			       tp = load_out->ContainerType();
		cPosition			  pos = get_context()->human->getInterfaceToWoLA()->get_pilon_local_position(load_out->get_current_pilon());
		IR_missile_seeker->setElecPower(true);
		IR_missile_seeker->setPlatformInitialLocalPosition(pos);
		IR_missile_seeker->checkInSearch(IR_missile_seeker->getElecPower());
	}
	else if (IR_missile_seeker)
		IR_missile_seeker->clear();
	if (IR_missile_seeker)
	{
		cPosition pos;
		pos.rotY(params[WS_IR_MISSILE_SEEKER_DESIRED_AZIMUTH]);
		pos.rotZ(params[WS_IR_MISSILE_SEEKER_DESIRED_ELEVATION]);

		if (IR_missile_seeker->getTarget().world_id != 0)
		{
			if (IR_missile_seeker->getStatusID() != IRSNSR_TRACK)
				IR_missile_seeker->setStatusID(IRSNSR_TRACK);
			params[WS_IR_MISSILE_LOCK] = 1.0;
		}
		else
		{	
			if (IR_missile_seeker->getStatusID() != SWSKR_SLAVED)
			{
				IR_missile_seeker->clear();
				IR_missile_seeker->setStatusID(SWSKR_SLAVED);
			}
			IR_missile_seeker->slaveToDirection(pos.x);
			params[WS_IR_MISSILE_LOCK] = 0;
		}
		IR_missile_seeker->update(sight_timer->GetDeltaTime());

		dVector direction  = IR_missile_seeker->getPlatformLocalPosition().x;
		
		PolarPosition p(direction);

		params[WS_IR_MISSILE_TARGET_AZIMUTH]   = p.azimuth;
		params[WS_IR_MISSILE_TARGET_ELEVATION] = p.elevation;
	}
}
