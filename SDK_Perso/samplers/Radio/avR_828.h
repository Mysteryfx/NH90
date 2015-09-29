#ifndef AV_R_828_H
#define AV_R_828_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "CockpitBase.h"

#include "ICommandsCockpitDevice.h"
#include "Avionics/Radio/avBaseRadio.h"

#include "Randomizer/RandomObject.h"

namespace EagleFM { namespace Ka50 {
	class Ka50ElectricSystem;
} }

namespace cockpit { 

class ASUSearchTimer;

// Attitude and Heading Reference System
class COCKPITBASE_API avR_828 :	public avBaseRadio					
{
	typedef enum avR_828_commands
	{
		CMD_R_828_CHANNEL_CHANGE = iCommandCockpitDevice_Action_1,
		CMD_R_828_VOLUME_CHANGE,
		CMD_R_828_ASU,
		CMD_R_828_NOISE_REDUCTOR_ON_OFF2,
		CMD_R_828_POWER_ON_OFF2
	};

public:
	avR_828();
	virtual ~avR_828();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	virtual void SetCommand(int, float value = 0);
	//end of interface

	bool	getSearchLampStatus() const {return ElecCon_Search_Lamp.GetVoltage() > 0.0;}

protected:

	virtual void	perform_init_state();

	void			setChannel_(unsigned int channel_);
	virtual float	get_set_frequency() const;
	virtual void	tuning_finished();

	void			connect_electric(EagleFM::Elec::ItemBase& wire);

	void			setSearchLampStatus(bool on_off_) {ElecSwitch_Search_Lamp.setOnOff(on_off_);}

	void			switchASU(bool asu_) {asu = asu_;}	
	bool			checkASU() const {return asu;}

	virtual const char*	l_metatable_name() const { return "R828_meta";}
	virtual void		l_register_functions(lua_State *Lua_s,int index);
	static int			l_set_channel(lua_State *Lua_s);

	EagleFM::Elec::Switch	ElecSwitch_Search_Lamp;
	EagleFM::Elec::Consumer	ElecCon_Search_Lamp;

	static RandomObject randomEvenly; // равномерное (0, 1)

	bool asu;	
};

} 

#endif // _avR_828