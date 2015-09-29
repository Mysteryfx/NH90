#pragma once

#include "Avionics/Radio/avBaseRadio.h"
#include "Avionics/Radio/FrequencyDial.h"

#include "ICommandsCockpitDevice.h"
#include "Sound/Sounder.h"

namespace cockpit {

class COCKPITBASE_API avVHF_ARC_186 : public avBaseRadio
{
protected:
	enum enum_avVHF_186_commands
	{
		VHF_CMD_PRESET_CHANNEL = iCommandCockpitDevice_Action_1,
		VHF_CMD_EMPTY,
		VHF_CMD_FREQ_MODE_DIAL,
		VHF_CMD_FREQ_SELECTION_DIAL,
		VHF_CMD_VOLUME,
		VHF_CMD_LOAD,
		VHF_CMD_SQUELCH,
		VHF_CMD_SQUELCH_TONE,
		VHF_CMD_FREQ_SELECTOR_1_INC,
		VHF_CMD_FREQ_SELECTOR_1_DEC,
		VHF_CMD_FREQ_SELECTOR_2_INC,
		VHF_CMD_FREQ_SELECTOR_2_DEC,
		VHF_CMD_FREQ_SELECTOR_3_INC,
		VHF_CMD_FREQ_SELECTOR_3_DEC,
		VHF_CMD_FREQ_SELECTOR_4_INC,
		VHF_CMD_FREQ_SELECTOR_4_DEC,
		VHF_CMD_PRESET_CHANNEL_EXT,
	};

	enum enum_VHF_186_FunctionalModeDial
	{
		/*Do not change sequence*/
		VHF_FMD_OFF = 0,
		VHF_FMD_TR,
		VHF_FMD_DF
	};

	enum enum_VHF_186_FrequencySelectionDial
	{ 
		/*Do not change sequence*/
		VHF_FSD_EMER_FM = 0,
		VHF_FSD_EMER_AM,
		VHF_FSD_MAN,
		VHF_FSD_PRE
	};

	enum enum_VHF_186_Squelch
	{
		/*Do not change sequence*/
		VHF_SQ_DIS = -1,
		VHF_SQ_ON = 0,
		VHF_SQ_TONE = 1
	};

public:

	enum VHF_186_digits 
	{
		VHF186_DEC_DIGIT = 0,
		VHF186_ONES_DIGIT,
		VHF186_TENS_DIGIT,
		VHF186_HUNDTH_DIGIT,
		VHF186_MAX_DIGIT
	};

	avVHF_ARC_186();
	virtual ~avVHF_ARC_186();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void SetCommand(int command, float value = 0);
	//end of interface

	unsigned char get_digit(VHF_186_digits position) const { return frequency_dialer_[position].value_; }

protected:

	void			load_freq_dialer(Lua::Config& config);
	virtual void	checkRadioDevices();
	void			connect_electric(EagleFM::Elec::ItemBase& wire);
	
	virtual void	perform_init_state();

	virtual void	on_mode_change(int newMode);

	virtual int		get_set_frequency() const override; 
	int				get_knobs_frequency() const;

	void			set_knobs_frequency(int frequency);
	virtual void	set_preset_channel(unsigned int channel) override;

	void			knobs_frequency_changed();

	virtual void	on_frequency_changed() override;

	void			update_power_and_tone();

	bool			isAMRadio() const;

	//scripting
	virtual void 	ext_set_frequency(int frequency) override;

	virtual	void	load_dials(Lua::Config& config) override;
	virtual void	save_dials() const override;

protected:

	unsigned char functional_mode_;
	unsigned char frequency_mode_;

	Sounder::Source warningTone_;

	struct ARC186Params
	{
		ARC186Params() 
			: noise_k(0.0f), power(0.0f), innerNoise(0.0f){}
		float	noise_k;
		float	power;
		float	innerNoise;
		int		EMER_FREQUENCY_;
	} params_;

	FrequencyDial frequency_dialer_[VHF186_MAX_DIGIT];
};

}
