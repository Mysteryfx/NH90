#include "StdAfx.h"
#include "Avionics/RWR/avSimpleRWR.h"
#include "IView.h"
#include "vPublic.h"
#include "IwHumanPlane.h"
#include "WorldManager.h"
#include "wsType_Level4.h"
#include "Lua/Config.h"
#include "ccSound.h"
#include "ccModuleManager.h"
#include "ed_log.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avSimpleRWR, avSimpleRWR);

avSimpleRWR::avSimpleRWR()
{
}

avSimpleRWR::~avSimpleRWR()
{
}

void avSimpleRWR::initialize(unsigned char ID, const std::string& Name, const std::string& script_name)
{
	avRWR::initialize(ID, Name, script_name);

	Lua::Config config(ensured_Lua("avSimpleRWR"));

	std::string power_bus;

	if (config.get("power_bus_handle",&power_bus))
	{
		param_bus.setNominalVoltage(28);
		param_bus.init_with(power_bus.c_str());
	}
	
	power_switch >> power_line;
}

void avSimpleRWR::update()
{
	if (!param_bus.empty())
		 param_bus.update_connection();

	avRWR::update();

	update_export();	
	
	avLuaDevice::update();
}


void  avSimpleRWR::post_initialize()
{
	avRWR::post_initialize();

	if (!param_bus.empty())
		 param_bus >> power_switch;
	else if (avSimpleElectricSystem::get())
			 avSimpleElectricSystem::get()->getACbus(1) >> power_switch; 
}


void  avSimpleRWR::update_export()
{
	int j = 0;
	for (int i = 0;
			 i < emitters.size(); 
		   ++i)
	{
		if (!emitters[i].IsValid)
			continue;
		if (contacts_exported.size() < j + 1)
			contacts_exported.resize  (j + 1);
		contacts_exported[j].update_with_emitter(emitters[i],j + 1);
		++j;

	}
	for (; j < contacts_exported.size(); ++j)
	{
		contacts_exported[j].mark_invalid();
	}
}

void RWR_contact_export::update_with_emitter(const RWR_Emitter & contact, int index)
{
	if (life_time_handle.empty())
	{
		std::string name("RWR_CONTACT_");
		char buffer[8];
		sprintf_s(buffer,8,"%02d_",index);
		name += buffer;

		life_time_handle	.init_with((name + "TIME").c_str());
		azimuth_handle		.init_with((name + "AZIMUTH").c_str());
		elevation_handle	.init_with((name + "ELEVATION").c_str());
		unit_type_handle	.init_with((name + "UNIT_TYPE").c_str());
		signal_handle		.init_with((name + "SIGNAL").c_str());
		signal_power		.init_with((name + "POWER").c_str());
		signal_priority		.init_with((name + "PRIORITY").c_str());
		signal_source		.init_with((name + "SOURCE").c_str());
		signal_general_type .init_with((name + "GENERAL_TYPE").c_str());
	}
	double dt = wTime::GetModelTime() - contact.BirthTime;

	if (dt < 1e-3)
		dt = 0.001;

	life_time_handle	= dt;
	azimuth_handle		= contact.Azimuth;
	elevation_handle	= contact.Elevation;
	unit_type_handle	= contact.EmitterTypeStr;
	signal_power		= contact.SignalStreight;
	signal_priority		= contact.Priority;
	signal_handle		= (double)contact.SignalType;
	signal_source		= (double)contact.client;
	signal_general_type = (double)contact.PlatformType;
}


void RWR_contact_export::mark_invalid()
{
	life_time_handle	= -1.0;
	signal_power		= 0.0;
	signal_handle		= 0.0;
}

void avSimpleRWR::l_register_functions( lua_State *Lua_s,int index )
{
	avLuaDevice::l_register_functions(Lua_s,index);
	lua_pushstring(Lua_s, "get_power");	lua_pushcfunction(Lua_s,l_get_power);	lua_settable  (Lua_s,index);
	lua_pushstring(Lua_s, "set_power");	lua_pushcfunction(Lua_s,l_set_power);	lua_settable  (Lua_s,index);
	lua_pushstring(Lua_s, "reset");		lua_pushcfunction(Lua_s,l_reset);		lua_settable  (Lua_s,index);
}

int avSimpleRWR::l_set_power( lua_State *Lua_s )
{
	avSimpleRWR  *device = l_get_device_from_meta<avSimpleRWR>(Lua_s);
	if (device)
		device->setElecPower(lua_toboolean(Lua_s,2));
	return  0;
}

int avSimpleRWR::l_get_power( lua_State *Lua_s )
{
	avSimpleRWR  *device = l_get_device_from_meta<avSimpleRWR>(Lua_s);
	lua_pushboolean(Lua_s, device && device->getElecPower());
	return  1;
}

int avSimpleRWR::l_reset(lua_State *Lua_s)
{
	avSimpleRWR  *device = l_get_device_from_meta<avSimpleRWR>(Lua_s);
	if (device)
	{
		device->emitters.clear();
	}
	return  0;
}

