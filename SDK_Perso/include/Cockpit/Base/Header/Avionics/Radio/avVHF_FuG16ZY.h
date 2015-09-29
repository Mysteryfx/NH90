#pragma once

#include "Avionics/Radio/avBaseRadio.h"

#include "ICommandsCockpitDevice.h"

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/relay.h"

#include "Avionics/Radio/avADF.h"

namespace cockpit
{
class avIntercomWWII;

class COCKPITBASE_API avVHF_FuG16ZY: public avBaseRadio
{

private:
	enum FuG16ZY_Commands
	{
		C_FuG16ZY_CHANNEL = iCommandCockpitDevice_Action_1,
		C_FuG16ZY_CHANNEL_EXT,
		C_FuG16ZY_VOLUME,
		C_FuG16ZY_VOLUME_EXT,
		C_FuG16ZY_MIC,
		C_FuG16ZY_MODE,
		C_FuG16ZY_MODE_EXT,
		C_FuG16ZY_TUNE,
		C_FuG16ZY_TUNE_EXT,
	};

	enum enum_FuG16ZY_Functional_Mode
	{
		FuG16ZY_Y_ZF = 0,
		FuG16ZY_FT_FT,
		FuG16ZY_Mode
	};

	enum enum_FuG16ZY_Channel
	{
		FuG16ZY_CH_1 = 0,
		FuG16ZY_CH_2,
		FuG16ZY_CH_3,
		FuG16ZY_CH_4
	};

public:
	avVHF_FuG16ZY();

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& name, const ed::string& script_name);
	virtual void SetCommand(int command, float value = 0);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	virtual void update();
	virtual void release();
	//end of interface

	//avBreakable interface
	virtual bool	set_damage(unsigned Failure_ID, const bool can_be_rejected = false);
	virtual void	repair();
	//end of interface

	void	mic_button_pressed(bool pressed); 

	float get_signal_strenght() const { return signal_strenght_.get_value_out(); }
	float get_deviation() const { return signal_deviation_.get_value_out(); }

protected:

	void	connect_electric(EagleFM::Elec::Wire& power_wire);
	virtual void	set_preset_channel(unsigned int channel) override;

private:

	DECLARE_DEV_LINKPTR(cockpit::avIntercomWWII, Intercom);

	virtual void	checkRadioDevices();
	virtual	void	perform_init_state();

	void			load_presets();

	void			onFunctionalModeChanging(int newMode);

	virtual int		get_set_frequency() const;
	void			setKnobsByFrequency(float frequency);
	void			setKnobsByChannel(int channel);

	void			update_communicator_state();

	virtual const char*	l_metatable_name() const { return "FuG_16ZY_meta";}
	virtual void		ext_set_channel(int channel);
	virtual bool		ext_is_on() const;

	virtual	void	load_dials(Lua::Config& config);
	virtual void	save_dials() const; 

	EagleFM::Elec::Switch	receiving_switch_;

	unsigned char functional_mode_;
	unsigned char sel_channel_;

	bool	mic_button_pressed_;
	float	anim_volume_;
	int		delta_freq_;

	avADF ADF_;

	gauge_limited signal_strenght_;
	gauge_limited signal_deviation_;
};

}