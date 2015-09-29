#ifndef AV_R_828_H
#define AV_R_828_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "CockpitBase.h"

#include "ICommandsCockpitDevice.h"
#include "Avionics/Radio/avBaseRadio.h"

#include "FMElectricSystems/Items/Lamp.h"
#include "Randomizer/RandomObject.h"

namespace cockpit { 

// Attitude and Heading Reference System
class COCKPITBASE_API avR_828 :	public avBaseRadio					
{
protected:
	typedef enum avR_828_commands
	{
		CMD_R_828_CHANNEL_CHANGE = iCommandCockpitDevice_Action_1,
		CMD_R_828_VOLUME_CHANGE,
		CMD_R_828_ASU,
		CMD_R_828_NOISE_REDUCTOR_ON_OFF2,
		CMD_R_828_POWER_ON_OFF2,
		CMD_R_828_MAX
	};

public:
	avR_828();
	virtual ~avR_828();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	virtual void SetCommand(int, float value = 0);
	//end of interface

	float	getSearchLampStatus() const {return ElecCon_Search_Lamp.getLightness(); }

protected:

	virtual void	perform_init_state();

	virtual void	set_preset_channel(unsigned int channel);

	virtual int		get_set_frequency() const;
	virtual void	tuning_finished();

	void			connect_electric(EagleFM::Elec::ItemBase& wire);

	void			setSearchLampStatus(bool on_off_) {ElecSwitch_Search_Lamp.setOnOff(on_off_);}

	void			switchASU(bool asu_) {asu_active_ = asu_;}	
	bool			checkASU() const {return asu_active_;}

	virtual const char*	l_metatable_name() const { return "R828_meta";}
	virtual void		ext_set_channel(int channel);

	EagleFM::Elec::Switch	ElecSwitch_Search_Lamp;
	EagleFM::Elec::Lamp		ElecCon_Search_Lamp;

	bool asu_active_;	

	virtual	void	load_dials(Lua::Config& config);
	virtual void	save_dials() const; 
};

} 

#endif // _avR_828