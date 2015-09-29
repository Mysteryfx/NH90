#include "stdafx.h"

#include "Avionics/Radio/avUHF_ARC_164.h"
#include "ccModuleManager.h"
#include "ccClickableElementData.h"
#include "Utilities/ccTriggerSystem.h"

REGISTER_DYNAMIC_FACTORY(cockpit::avUHF_ARC_164, avUHF_ARC_164);

using namespace cockpit;

avUHF_ARC_164::avUHF_ARC_164()
	:avUHF_Radio()
{
}

avUHF_ARC_164::~avUHF_ARC_164()
{
}

void avUHF_ARC_164::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avUHF_Radio::initialize(ID, Name, script_name);

	testDisplay_ = false;
	testing_start_time_ = 0.0;

	presetChannelLever = 0.0;

	knobs_frequency[DIGIT_100MHz]	= UHF_100MHz_2;
	knobs_frequency[DIGIT_10MHz]	= 0;
	knobs_frequency[DIGIT_1MHz]		= 0;
	knobs_frequency[DIGIT_01MHz]	= 0;
	knobs_frequency[DIGIT_0025MHz]  = 0;

	listen_command_range(this,iCommandPlaneUHFPresetChannelSelectorInc,iCommandPlaneUHFPanelCover);
	listen_command_range(this,iCommandUHFAntennaUpper,iCommandEGI_HQ_TOD);
}

void avUHF_ARC_164::post_initialize()
{
	avUHF_Radio::post_initialize();
	performClickableAction(UHF_CMD_SQUELCH, 0.0f);
	// Hide "Load" button at startup
	clickableElementData* elem = getClickableElement(UHF_CMD_LOAD_BUTTON);
	elem->hide(true);

	if (aircraft().init_conditions.cpt_init_condition != CPT_INCON_COLD_GROUND)
	{	
		performClickableAction(UHF_CMD_FUNCTION_DIAL, float(UHF_FD_MAIN) * 0.1f);
		performClickableAction(UHF_CMD_FREQ_MODE_DIAL, float(UHF_FMD_PRESET) * 0.1f);
	}
	else // cold start
	{
		performClickableAction(UHF_CMD_FUNCTION_DIAL, float(UHF_FD_OFF) * 0.1f);
		performClickableAction(UHF_CMD_FREQ_MODE_DIAL, float(UHF_FMD_MNL) * 0.1f);
	}

	if(is_load_from_mission)
	{
		forceSetArgument(UHF_CMD_FUNCTION_DIAL, float(functionDial) * 0.1f);
		forceSetArgument(UHF_CMD_FREQ_MODE_DIAL, float(freqModeDial) * 0.1f);
		forceSetArgument(UHF_CMD_PRESET_CHANNEL,(presetChannel-1)/100.0f);			
	}

	communicator.setFrequency(presetChannels[presetChannel]);
	setKnobsFrequency(presetChannels[presetChannel]);
	performClickableAction(UHF_CMD_PRESET_CHANNEL,	    (presetChannel - 1)  / 20.0f);
}

void avUHF_ARC_164::SetCommand(int command, float value)
{
	switch (command)
	{
	case iCommandPlaneUHFPresetChannelSelectorDec:
		switcher_value_iterate(UHF_CMD_PRESET_CHANNEL,-1);
		break;
	case iCommandPlaneUHFPresetChannelSelectorInc:
		switcher_value_iterate(UHF_CMD_PRESET_CHANNEL,1);
		break;
	case UHF_CMD_PRESET_CHANNEL :
		{
			int preset_channel = cockpit::round(value * 20) + 1;

			setPresetChannel(preset_channel);

			if(freqModeDial == UHF_FMD_PRESET)
					onFrequencyChanged();			
		}
		break;

	case iCommandPlaneUHF100MhzSelector1:
		performClickableAction(UHF_CMD_FREQ_SELECTOR_100MHZ,0.0f);
		break;
	case iCommandPlaneUHF100MhzSelector2:
		performClickableAction(UHF_CMD_FREQ_SELECTOR_100MHZ,0.1f);
		break;
	case iCommandPlaneUHF100MhzSelector3:
		performClickableAction(UHF_CMD_FREQ_SELECTOR_100MHZ,0.2f);
		break;
	case iCommandPlaneUHF100MhzSelectorA:
		performClickableAction(UHF_CMD_FREQ_SELECTOR_100MHZ,0.3f);
		break;
	case UHF_CMD_FREQ_SELECTOR_100MHZ :
		{
			knobs_frequency[DIGIT_100MHz] = cockpit::round(value * 10) + 2;		
			setManualSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlaneUHF10MhzSelectorDec:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_10MHZ,-1);
		break;
	case iCommandPlaneUHF10MhzSelectorInc:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_10MHZ,1);
		break;

	case UHF_CMD_FREQ_SELECTOR_10MHZ:
		{
			knobs_frequency[DIGIT_10MHz] = cockpit::round(value * 10);
			setManualSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlaneUHF1MhzSelectorDec:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_1MHZ,-1);
		break;
	case iCommandPlaneUHF1MhzSelectorInc:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_1MHZ,1);
		break;
	case UHF_CMD_FREQ_SELECTOR_1MHZ :
		{
			knobs_frequency[DIGIT_1MHz] = cockpit::round(value * 10);
			setManualSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlaneUHF01MhzSelectorDec:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_01MHZ,-1);
		break;
	case iCommandPlaneUHF01MhzSelectorInc:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_01MHZ,1);
		break;
	case UHF_CMD_FREQ_SELECTOR_01MHZ:
		{
			knobs_frequency[DIGIT_01MHz] = cockpit::round(value * 10);
			setManualSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlaneUHF0025MhzSelectorDec:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_0025MHZ,-1);
		break;
	case iCommandPlaneUHF0025MhzSelectorInc:
		switcher_value_iterate(UHF_CMD_FREQ_SELECTOR_0025MHZ,1);
		break;
	case UHF_CMD_FREQ_SELECTOR_0025MHZ:
		{
			knobs_frequency[DIGIT_0025MHz] = cockpit::round(value * 10) * 25;
			setManualSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlaneUHFFreqModeDialMNL:
		performClickableAction(UHF_CMD_FREQ_MODE_DIAL,0.0f);
		break;
	case iCommandPlaneUHFFreqModeDialPRESET:
		performClickableAction(UHF_CMD_FREQ_MODE_DIAL,0.1f);
		break;
	case iCommandPlaneUHFFreqModeDialGRD:
		performClickableAction(UHF_CMD_FREQ_MODE_DIAL,0.2f);
		break;
	case UHF_CMD_FREQ_MODE_DIAL :
		freqModeDial = cockpit::round(value * 10.0);
		onFrequencyChanged();
		break;

	case iCommandPlaneUHFFunctionDialOFF:
		performClickableAction(UHF_CMD_FUNCTION_DIAL,0.0f);
		break;
	case iCommandPlaneUHFFunctionDialMAIN:
		performClickableAction(UHF_CMD_FUNCTION_DIAL,0.1f);
		break;
	case iCommandPlaneUHFFunctionDialBOTH:
		performClickableAction(UHF_CMD_FUNCTION_DIAL,0.2f);
		break;
	case iCommandPlaneUHFFunctionDialADF:
		performClickableAction(UHF_CMD_FUNCTION_DIAL,0.3f);
		break;
	case UHF_CMD_FUNCTION_DIAL :
		{
			int newFunction = cockpit::round(value * 10.0);

			if (newFunction != functionDial)
				onFunctionDialChanging(newFunction);

		}
		break;

	case iCommandPlaneUHFTestDisplay:
		button_value_iterate(UHF_CMD_TEST_DISPLAY);
		break;
	case UHF_CMD_TEST_DISPLAY :
		testDisplay_ = !testDisplay_;
		break;
	case iCommandPlaneUHFStatus:
		button_value_iterate(UHF_CMD_FREQ_STATUS);
		break;
	case UHF_CMD_FREQ_STATUS :
		if (!isAlternateFreqStatus())
			alternateFrequencyStatus = wTime::GetModelTime();
		break;
	case iCommandPlaneUHFVolumeDec:
		axis_value_iterate(UHF_CMD_VOLUME,-1);
		break;
	case iCommandPlaneUHFVolumeInc:
		axis_value_iterate(UHF_CMD_VOLUME,1);
		break;
	case UHF_CMD_VOLUME:
		communicator.setVolume(value);
		break;

	case iCommandPlaneUHFSquelch:
		switcher_value_iterate(UHF_CMD_SQUELCH);
		break;
	case UHF_CMD_SQUELCH:
		communicator.setSquelchOnOff(value < 0.5f);
		break;

	case iCommandPlaneUHFPanelCover:
		switcher_value_iterate(UHF_CMD_COVER);
		break;
	case UHF_CMD_COVER:
		{
         clickableElementData* elem = getClickableElement(UHF_CMD_LOAD_BUTTON);
		 if(elem)
			 elem->hide(value == 0.0);
		}
		break;

	case iCommandPlaneUHFLoad:
		button_value_iterate(UHF_CMD_LOAD_BUTTON);
		break;
	case UHF_CMD_LOAD_BUTTON:
		if (freqModeDial == UHF_FMD_PRESET && getElecPower())
		{
			presetChannels[presetChannel] = getKnobsFrequency();
		}
		break;
	case  iCommandUHFAntennaLower:
		performClickableAction(UHF_CMD_ANT_UPPER,0.0f);
		break;
	case  iCommandUHFAntennaAuto:
		performClickableAction(UHF_CMD_ANT_UPPER,0.5f);
		break;
	case  iCommandUHFAntennaUpper:
		performClickableAction(UHF_CMD_ANT_UPPER,1.0f);
		break;
	case  UHF_CMD_ANT_UPPER:
		break;
	case  iCommandEGI_HQ_TOD:
		switcher_value_iterate(UHF_CMD_EGI_HQ_TOD);
		break;
	case  UHF_CMD_EGI_HQ_TOD:
		break;
	case UHF_CMD_T_TONE:
		transmit_DF_tone(value > 0.5);
		break;
	}
}

void avUHF_ARC_164::setKnobsFrequency(double frequency)
{
	knobs_frequency[DIGIT_100MHz] = floor(frequency / 100000000.0);
	frequency -= knobs_frequency[DIGIT_100MHz] * 100000000.0;

	knobs_frequency[DIGIT_10MHz] = floor(frequency /  10000000.0);
	frequency -= knobs_frequency[DIGIT_10MHz] * 10000000.0;

	knobs_frequency[DIGIT_1MHz] = floor(frequency /  1000000.0);
	frequency -= knobs_frequency[DIGIT_1MHz] * 1000000.0;

	knobs_frequency[DIGIT_01MHz] = floor(frequency /  100000.0);
	frequency -= knobs_frequency[DIGIT_01MHz] * 100000.0;

	knobs_frequency[DIGIT_0025MHz] = floor(frequency /  1000.0);
	frequency -= knobs_frequency[DIGIT_0025MHz] * 1000.0;

	performClickableAction(UHF_CMD_FREQ_SELECTOR_100MHZ,(knobs_frequency[DIGIT_100MHz] - 2) / 10.0f );
	performClickableAction(UHF_CMD_FREQ_SELECTOR_10MHZ,	knobs_frequency[DIGIT_10MHz] / 10.0f );
	performClickableAction(UHF_CMD_FREQ_SELECTOR_1MHZ,	knobs_frequency[DIGIT_1MHz]  / 10.0f );
	performClickableAction(UHF_CMD_FREQ_SELECTOR_01MHZ,	knobs_frequency[DIGIT_01MHz] / 10.0f );
	performClickableAction(UHF_CMD_FREQ_SELECTOR_0025MHZ,knobs_frequency[DIGIT_0025MHz] / 250.0f );
}

void avUHF_ARC_164::forceSetArgument(int command, double val)
{
	clickableElementData* elem = getClickableElement(command);

	if (!elem)
		return;

	for (int i = 0;i < TYPES_NUMBER;++i)
	{
		clickableElementActionData & action_data = elem->getActionData(i);
		if (action_data.command != command)
			continue;

		//action_data.drawingIndexValue = val;
		get_context()->set_argument(action_data.drawingIndex, val);
	}
}

void avUHF_ARC_164::setExternalFrequency(double frequency)
{
	performClickableAction(UHF_CMD_FREQ_MODE_DIAL, float(UHF_FMD_MNL) * 0.1f);
	setManualSelectedFrequency(frequency);
	setKnobsFrequency(frequency);
}

double avUHF_ARC_164::getKnobsFrequency() const
{
	return getKnobsFrequencyMHz() * 1000000 + getKnobsFrequencyKHz() * 1000;
}

int avUHF_ARC_164::getKnobsFrequencyMHz() const
{
	return knobs_frequency[DIGIT_100MHz] * 100 + knobs_frequency[DIGIT_10MHz] * 10 + knobs_frequency[DIGIT_1MHz];
}

int avUHF_ARC_164::getKnobsFrequencyKHz() const
{
	return knobs_frequency[DIGIT_01MHz] * 100 + knobs_frequency[DIGIT_0025MHz];
}

int avUHF_ARC_164::getNetAddress()
{
	return knobs_frequency[DIGIT_10MHz] * 10000 + knobs_frequency[DIGIT_1MHz] * 1000 + 
		knobs_frequency[DIGIT_01MHz] * 100 + knobs_frequency[DIGIT_0025MHz];
}

void avUHF_ARC_164::onFunctionDialChanging(int newFunction)
{
	avUHF_Radio::onFunctionDialChanging(newFunction);

	if (getElecPower())
	{
		if (newFunction != UHF_FD_OFF)
		{
			if (functionDial == UHF_FD_OFF)
			{
				testing_start_time_ = wTime::GetModelTime();
			}
		}
	}
}