#pragma once

#include "Avionics/Radio/avVHF_Radio.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit {

class COCKPITBASE_API avVHF_ARC_186 : public avVHF_Radio
{
protected:
	enum enum_avVHF_186_commands
	{
		VHF_CMD_PRESET_CHANNEL_INC = iCommandCockpitDevice_Action_1,
		VHF_CMD_PRESET_CHANNEL_DEC,
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
		VHF_CMD_FREQ_SELECTOR_4_DEC
	};

	enum enum_VHF_186_FrequencyModeDial
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
	avVHF_ARC_186();
	virtual ~avVHF_ARC_186();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	void SetCommand(int command, float value = 0);
	//end of interface
	
	void load_settings();
	void save_settings(); // save settings to mission file

	virtual bool checkAnimationConditions(int arg_num, int command, float& value);

	int getFreq_1st_Digit() { return freq_1st_digit; }
	int getFreq_2nd_Digit() { return freq_2nd_digit; }
	int getFreq_3rd_Digit() { return freq_3rd_digit; }
	int getFreq_4th_Digit() { return freq_4th_digit; }

protected:
	virtual float	getFrequency() const;
	virtual void	setSelectedFrequency(float frequency);

	float			getKnobsFrequency() const;
	void			setKnobsFrequency(float fq);
	void			forceSetArgument(int command, double val);

protected:
	int freq_1st_digit;
	int freq_2nd_digit;
	int freq_3rd_digit;
	int freq_4th_digit;

	const float EMER_FM_FQ;
	const float EMER_AM_FQ;

};

}
