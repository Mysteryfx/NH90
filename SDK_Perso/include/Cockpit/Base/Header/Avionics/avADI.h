#pragma once

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "avMechanics.h"
#include "Avionics/avSlipBall.h"

namespace cockpit
{

class ccPanelGauge;

class COCKPITBASE_API avADI : public avDevice,
						  	  public avBreakable, 
							  public avBasicElectric
{
public:
	avADI();
	virtual ~avADI();
	
	//avDevice interface
	virtual void release();
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
    virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	//end of interface

	virtual double get_pitch()    const {return pitch.get_value_out();}
	virtual double get_bank()     const {return bank.get_value_out();}
	double         get_sideslip() const {return SlipBall.get_sideslip();}

    virtual void   update();
protected:
	const char * l_metatable_name() const { return "adi_meta";}
	void		 l_register_functions(lua_State *Lua_s,int index);

	virtual	void read_gauges(Lua::Config & config);
	static int l_get_adi_pitch(lua_State *Lua_s);
	static int l_get_adi_bank(lua_State *Lua_s);
	static int l_get_slipball_sideslip(lua_State *Lua_s);
	
	gauge_limited       pitch;
	gauge_periodic	    bank;

protected:
	avSlipBall          SlipBall;

	ed::vector<ccPanelGauge*> gauges; //optional gauges creation
};

}
