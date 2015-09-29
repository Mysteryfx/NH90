#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "avMechanics.h"

#include "FMElectricSystems/Items/Consumer.h"

namespace cockpit
{

class COCKPITBASE_API avHSI :   public avDevice,
								public avBreakable, 
								public avBasicElectric
{
public:
	avHSI();
	virtual ~avHSI();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	//end of interface

	double get_heading() const {return heading.get_value_out();};
	virtual bool   getElecPower() {return ElecConDevice.isPowered();}

protected:
	const char * l_metatable_name() const { return "hsi_meta";}
	void		 l_register_functions(lua_State *Lua_s,int index);
	static int   l_get_heading(lua_State *Lua_s);
	
	void OnPowerOff(const lwl::CSignalInfo* pInfo);

	EagleFM::Elec::Consumer	ElecConDevice;
	gauge_periodic heading;
};

}
