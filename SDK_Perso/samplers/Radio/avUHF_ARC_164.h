#pragma once

#include "Avionics/Radio/avUHF_Radio.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit {

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

class COCKPITBASE_API avUHF_ARC_164 : public avUHF_Radio
{
public:
	avUHF_ARC_164();
	virtual ~avUHF_ARC_164();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	void SetCommand(int command, float value = 0);
 	//end of interface

	bool	displayTesting() { return testDisplay_ ||  wTime::GetModelTime() - testing_start_time_ < 1.5; }
	bool	isActiveMode() { return knobs_frequency[DIGIT_100MHz]  == (int)UHF_100MHz_A; }
	int		getKnobsFrequencyMHz() const;
	int		getKnobsFrequencyKHz() const;
	int		getNetAddress();

	virtual void setExternalFrequency(double frequency);

protected:

	virtual void onFunctionDialChanging(int newFunction);
	void	forceSetArgument(int command, double val);
	
	void	setKnobsFrequency(double frequency);
	double	getKnobsFrequency() const;

protected:

	enum Arc164Digits
	{
		DIGIT_100MHz = 0,
		DIGIT_10MHz,
		DIGIT_1MHz,
		DIGIT_01MHz,
		DIGIT_0025MHz,
	};

	bool testDisplay_;
	wModelTime	testing_start_time_; //for delayed indication start

	double presetChannelLever;

	int knobs_frequency[5];
};

}
