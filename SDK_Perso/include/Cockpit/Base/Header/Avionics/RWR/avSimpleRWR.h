#pragma once
#include "Avionics/RWR/avRWR.h"
#include "CockpitBase.h"
#include "Utilities/ccTriggerSystem.h"
#include "Avionics/avSimpleElectricSystem.h"

namespace cockpit 
{
	struct RWR_contact_export
	{
		triggers::double_param_proxy life_time_handle;
		triggers::double_param_proxy azimuth_handle;
		triggers::double_param_proxy elevation_handle;
		triggers::string_param_proxy unit_type_handle;
		triggers::double_param_proxy signal_handle;
		triggers::double_param_proxy signal_power;
		triggers::double_param_proxy signal_priority;
		triggers::double_param_proxy signal_source;
		triggers::double_param_proxy signal_general_type;



		void update_with_emitter(const RWR_Emitter & contact, int index);
		void mark_invalid();
	};

	class avSimpleRWR : public avRWR
	{
		avElectricSourceParamDriven param_bus;
		EagleFM::Elec::Switch		power_switch;
		EagleFM::Elec::Wire			power_line;

		ed::vector<RWR_contact_export>	contacts_exported;
		void update_export();
	public:
		avSimpleRWR();
		virtual ~avSimpleRWR();
		virtual void initialize(unsigned char, const ed::string&, const ed::string&);
		virtual void update();
		virtual void post_initialize();
		virtual bool getElecPower() const		{	return power_line.isPowered();}
		virtual void setElecPower(bool flag_in) {	power_switch.setOnOff(flag_in);	}


	protected:

		static int  l_set_power(lua_State *Lua_s);

		static int  l_get_power(lua_State *Lua_s);

		static int  l_reset(lua_State *Lua_s);

		void  l_register_functions( lua_State *Lua_s,int index );

	};
}