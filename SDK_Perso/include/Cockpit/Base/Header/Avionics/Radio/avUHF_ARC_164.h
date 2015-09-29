#pragma once

#include "Avionics/Radio/avBaseRadio.h"
#include "ICommandsCockpitDevice.h"
#include "Avionics/Radio/avADF.h"

namespace cockpit {

class COCKPITBASE_API avUHF_ARC_164 : public avBaseRadio            
{
protected:
	enum enum_avUHF_commands
	{
		UHF_CMD_PRESET_CHANNEL = iCommandCockpitDevice_Action_1,
		UHF_CMD_FREQ_SELECTOR_100MHZ,
		UHF_CMD_FREQ_SELECTOR_10MHZ,
		UHF_CMD_FREQ_SELECTOR_1MHZ,
		UHF_CMD_FREQ_SELECTOR_01MHZ,
		UHF_CMD_FREQ_SELECTOR_0025MHZ,
		UHF_CMD_FREQ_MODE_DIAL,
		UHF_CMD_FUNCTION_DIAL,
		UHF_CMD_T_TONE,
		UHF_CMD_SQUELCH,
		UHF_CMD_VOLUME,
		UHF_CMD_TEST_DISPLAY,
		UHF_CMD_FREQ_STATUS,
		UHF_CMD_COVER,
		UHF_CMD_LOAD_BUTTON,
		UHF_CMD_ANT_UPPER,
		UHF_CMD_EGI_HQ_TOD
	};

	enum enum_avUHF_100MHz
	{
		UHF_100MHz_T = 1,
		UHF_100MHz_2,
		UHF_100MHz_3,
		UHF_100MHz_A
	};

	enum enum_avUHF_Function_Dial
	{
		UHF_FD_OFF,
		UHF_FD_MAIN,
		UHF_FD_BOTH,
		UHF_FD_ADF
	};

	enum enum_avUHF_Freq_Mode_Dial
	{
		UHF_FMD_MNL,
		UHF_FMD_PRESET,
		UHF_FMD_GRD
	};

public:
	avUHF_ARC_164();
	virtual ~avUHF_ARC_164();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void release();
	virtual void update();
	virtual void SetCommand(int command, float value = 0);
 	//end of interface

	bool	displayTesting() { return testDisplay_ ||  wTime::GetModelTime() - testing_start_time_ < 1.5; }
	bool	isActiveMode() { return knobs_frequency[DIGIT_100MHz]  == (int)UHF_100MHz_A; }
	int		getKnobsFrequencyMHz() const;
	int		getKnobsFrequencyKHz() const;
	int		getNetAddress() const;
	bool	indicationOff() const {  return !getElecPower(); }

	float	getBearing() const;
	int		getAlternateFrequency() const ;
	bool	isAlternateFreqStatus() const;
	bool	isPresetChannelMode() const { return frequency_mode_ == UHF_FMD_PRESET; }

protected:
	virtual void	perform_init_state() override;
	void			connect_electric(EagleFM::Elec::ItemBase& wire);

	virtual void	checkRadioDevices() override;

	virtual int		get_set_frequency() const override; 
	int				get_knobs_frequency() const;
	void			set_knobs_frequency(int frequency); /*force knobs set*/

	virtual void	on_mode_change(int newMode);
	void			knobs_frequency_changed();
	virtual void	set_preset_channel(unsigned int channel) override;

	void			transmit_DF_tone(bool transmit);

	//scripting
	virtual const char*	l_metatable_name() const { return "UHF_ARC_164_meta";}
	virtual void 		ext_set_frequency(int frequency);

	virtual	void	load_dials(Lua::Config& config);
	virtual void	save_dials() const;

protected:

	avADF	ADF_;

	enum Arc164Digits
	{
		DIGIT_100MHz = 0,
		DIGIT_10MHz,
		DIGIT_1MHz,
		DIGIT_01MHz,
		DIGIT_0025MHz,
	};

	unsigned char functional_mode_;
	unsigned char frequency_mode_;

	double	alternateFrequencyStatus;
	bool	df_transmitting_;


	bool testDisplay_;
	wModelTime	testing_start_time_; //for delayed indication start

	double presetChannelLever;

	int knobs_frequency[5];
};

}
