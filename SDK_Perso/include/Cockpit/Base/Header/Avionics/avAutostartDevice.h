#pragma once 

#include "Avionics/avDevice.h"

namespace cockpit { 

class COCKPITBASE_API avAutostartDevice: public avDevice
{

public:
	virtual void l_register_functions(lua_State *Lua_s,int index) override;
	virtual void SetCommand( int command, float value /*= 0*/ ) override;

private:
	virtual int check_autostart_condition(unsigned int condition_id){ return 0; /* no faults */ };
	static int l_check_autostart_condition(lua_State *Lua_s);

};

}