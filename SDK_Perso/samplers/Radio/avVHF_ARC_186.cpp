#include "stdafx.h"

#include "Avionics/Radio/avVHF_ARC_186.h"
#include "ccModuleManager.h"
#include "ccClickableElementData.h"
#include "Utilities/ccTriggerSystem.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avVHF_ARC_186, avVHF_ARC_186);

avVHF_ARC_186::avVHF_ARC_186() 
	:EMER_FM_FQ(40500000.0f), EMER_AM_FQ(121500000.0f)
{
	functional_mode_ = VHF_FMD_OFF;
	selectionDial = VHF_FSD_MAN;
}

avVHF_ARC_186::~avVHF_ARC_186()
{
}

void avVHF_ARC_186::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avVHF_Radio::initialize(ID,Name,script_name);

	freq_1st_digit = 0;
	freq_2nd_digit = freq_3rd_digit = freq_4th_digit = 0;

	if(!isAMRadio())
		freq_1st_digit = 3; // default FM frequency 30 Mhz
	else
	{
		freq_1st_digit = 12; // default AM frequency 124 MHz
		freq_2nd_digit = 4;
	}

	if (isAMRadio())
	{
		listen_command_range(this,iCommandPlane_VHF_AM_Volume_Dec,iCommandPlane_VHF_AM_025MHz_Inc);
	}
	else
	{
		listen_command_range(this,iCommandPlane_VHF_FM_Volume_Dec,iCommandPlane_VHF_FM_025MHz_Inc);
	}

	for (int i = 1; i <= 20; i++)
		presetChannels[i] = isAMRadio() ? EMER_AM_FQ : EMER_FM_FQ;
}

void avVHF_ARC_186::post_initialize()
{
	avVHF_Radio::post_initialize();

	if(isAMRadio())
		performClickableAction(VHF_CMD_FREQ_SELECTOR_2_INC, 0.1f * freq_2nd_digit);

	performClickableAction(VHF_CMD_FREQ_SELECTOR_1_INC, 0.05f * freq_1st_digit);

	if (aircraft().init_conditions.cpt_init_condition != CPT_INCON_COLD_GROUND)
	{
		performClickableAction(VHF_CMD_FREQ_MODE_DIAL, float(VHF_FMD_TR) * 0.1f);
		performClickableAction(VHF_CMD_FREQ_SELECTION_DIAL, VHF_FSD_MAN * 0.1f);
		performClickableAction(VHF_CMD_SQUELCH, 0.0f);
	}
	else
	{
		performClickableAction(VHF_CMD_FREQ_MODE_DIAL, float(VHF_FMD_OFF) * 0.1f);
		performClickableAction(VHF_CMD_FREQ_SELECTION_DIAL, VHF_FSD_MAN * 0.1f);
		performClickableAction(VHF_CMD_SQUELCH, 0.0f);
	}

	if(is_load_from_mission)
	{
		if(selectionDial == VHF_FSD_PRE)
		{
			setSelectedFrequency( presetChannels[presetChannel]);
		}

		forceSetArgument(VHF_CMD_FREQ_MODE_DIAL, float(functional_mode_) * 0.1f);
		forceSetArgument(VHF_CMD_FREQ_SELECTION_DIAL, selectionDial * 0.1f);
		forceSetArgument(VHF_CMD_PRESET_CHANNEL_INC,(presetChannel-1)/100.0f);	
	}
}

void avVHF_ARC_186::SetCommand(int command, float value)
{
	switch (command)
	{ 
	case iCommandPlane_VHF_FM_Preset_Dec:
	case iCommandPlane_VHF_AM_Preset_Dec:
		performClickableAction(VHF_CMD_PRESET_CHANNEL_DEC,(presetChannel-1)/100.0f-0.01f);
		break;

	case iCommandPlane_VHF_FM_Preset_Inc:
	case iCommandPlane_VHF_AM_Preset_Inc:
		performClickableAction(VHF_CMD_PRESET_CHANNEL_INC,(presetChannel-1)/100.0f+0.01f);
		break;

	case VHF_CMD_PRESET_CHANNEL_INC :
	case VHF_CMD_PRESET_CHANNEL_DEC :
		{
			presetChannel = (int)cockpit::round(value * 100) % 20 + 1;
			if (presetChannel < 1)
				presetChannel += 20;

			if (selectionDial == VHF_FSD_PRE)
			{
				setSelectedFrequency(getKnobsFrequency());
			}
		}
		break;

	case iCommandPlane_VHF_AM_Mode_EMERFM:
	case iCommandPlane_VHF_FM_Mode_EMERFM:
		performClickableAction(VHF_CMD_FREQ_SELECTION_DIAL,0.0f);
		break;
	case iCommandPlane_VHF_AM_Mode_EMERAM:
	case iCommandPlane_VHF_FM_Mode_EMERAM:
		performClickableAction(VHF_CMD_FREQ_SELECTION_DIAL,0.1f);
		break;
	case iCommandPlane_VHF_AM_Mode_MAN:
	case iCommandPlane_VHF_FM_Mode_MAN:
		performClickableAction(VHF_CMD_FREQ_SELECTION_DIAL,0.2f);
		break;
	case iCommandPlane_VHF_AM_Mode_PRE:
	case iCommandPlane_VHF_FM_Mode_PRE:
		performClickableAction(VHF_CMD_FREQ_SELECTION_DIAL,0.3f);
		break;
	case VHF_CMD_FREQ_MODE_DIAL :
		{
			int newModeDial = (int)cockpit::round(value * 10);
			Power_Switch_.setOnOff(!(newModeDial == VHF_FMD_OFF));
			onFunctionalModeChanging(newModeDial);
		}
		break;

	case iCommandPlane_VHF_FM_FreqMode_OFF:
	case iCommandPlane_VHF_AM_FreqMode_OFF:
		performClickableAction(VHF_CMD_FREQ_MODE_DIAL,0.0f);
		break;
	case iCommandPlane_VHF_FM_FreqMode_TK:
	case iCommandPlane_VHF_AM_FreqMode_TK:
		performClickableAction(VHF_CMD_FREQ_MODE_DIAL,0.1f);
		break;
	case iCommandPlane_VHF_FM_FreqMode_DN:
	case iCommandPlane_VHF_AM_FreqMode_DN:
		performClickableAction(VHF_CMD_FREQ_MODE_DIAL,0.2f);
		break;
	case VHF_CMD_FREQ_SELECTION_DIAL :
		selectionDial = (int)cockpit::round(value * 10);
		setSelectedFrequency(getKnobsFrequency());
		break;
	case iCommandPlane_VHF_FM_10MHz_Dec:
	case iCommandPlane_VHF_AM_10MHz_Dec:
		{
			float new_value;
			if(freq_1st_digit == 3)
			{
				new_value = 0.15f;
			}
			else
			{
				new_value = freq_1st_digit / 20.0f - 0.05f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_1_INC,new_value);
		}
		break;
    case iCommandPlane_VHF_FM_10MHz_Inc:
	case iCommandPlane_VHF_AM_10MHz_Inc:
		{	
			float new_value;
			if(freq_1st_digit == 15)
			{
				new_value = 15 * 0.05f;
			}
			else
			{
				new_value = freq_1st_digit / 20.0f + 0.05f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_1_DEC,new_value);
		}
		break;

	case VHF_CMD_FREQ_SELECTOR_1_INC :
	case VHF_CMD_FREQ_SELECTOR_1_DEC :
		{
			freq_1st_digit = (int)cockpit::round(value * 20);
			setSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlane_VHF_FM_1MHz_Dec:
	case iCommandPlane_VHF_AM_1MHz_Dec:
		{
			float new_value;
			if (freq_2nd_digit == 0)
			{
				new_value = 9.0f/10.0f;
			}
			else
			{
				new_value = freq_2nd_digit/10.0f - 0.1f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_2_INC,new_value);
		}
		break;
	case iCommandPlane_VHF_FM_1MHz_Inc:
	case iCommandPlane_VHF_AM_1MHz_Inc:
		float new_value;
		if (freq_2nd_digit == 9)
		{
			new_value = .0f;
		}
		else
		{
			new_value = freq_2nd_digit/10.0f + 0.1f;
		}
		performClickableAction(VHF_CMD_FREQ_SELECTOR_2_DEC,new_value);
		break;
	case VHF_CMD_FREQ_SELECTOR_2_INC :
	case VHF_CMD_FREQ_SELECTOR_2_DEC :
		{
			freq_2nd_digit = int((value + 0.02) * 10) % 10;
			setSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlane_VHF_FM_01MHz_Dec:
	case iCommandPlane_VHF_AM_01MHz_Dec:
		{
			float new_value;
			if (freq_3rd_digit == 0)
			{
				new_value = 9.0f/10.0f;
			}
			else
			{
				new_value = freq_3rd_digit/10.f - 0.1f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_3_INC,new_value);
		}
		break;
	case iCommandPlane_VHF_FM_01MHz_Inc:
	case iCommandPlane_VHF_AM_01MHz_Inc:
		{
			float new_value;
			if (freq_3rd_digit == 9)
			{
				new_value = 0.0f;
			}
			else
			{
				new_value = freq_3rd_digit/10.f + 0.1f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_3_DEC,new_value);
		}
		break;
	case VHF_CMD_FREQ_SELECTOR_3_INC :
	case VHF_CMD_FREQ_SELECTOR_3_DEC :
		{
			freq_3rd_digit = int((value + 0.02) * 10) % 10;
			setSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlane_VHF_FM_025MHz_Dec:
	case iCommandPlane_VHF_AM_025MHz_Dec:
		{
			float new_value;
			if (freq_4th_digit == 0)
			{
				new_value = 0.75f;
			}
			else
			{
				new_value = freq_4th_digit/100.f - 0.25f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_4_INC,new_value);
		}
		
		break;
	case iCommandPlane_VHF_FM_025MHz_Inc:
	case iCommandPlane_VHF_AM_025MHz_Inc:
		{
			float new_value;
			if (freq_4th_digit == 75)
			{
				new_value = 0.0f;
			}
			else
			{
				new_value = freq_4th_digit/100.f + 0.25f;
			}
			performClickableAction(VHF_CMD_FREQ_SELECTOR_4_DEC,new_value );
		}
		
		break;
	case VHF_CMD_FREQ_SELECTOR_4_INC :
	case VHF_CMD_FREQ_SELECTOR_4_DEC :
		{		
			freq_4th_digit = int(value * 100) % 100;
			setSelectedFrequency(getKnobsFrequency());
		}
		break;

	case iCommandPlane_VHF_FM_Volume_Dec:
	case iCommandPlane_VHF_AM_Volume_Dec:
		axis_value_iterate(VHF_CMD_VOLUME,-1);
		break;
	case iCommandPlane_VHF_FM_Volume_Inc:
	case iCommandPlane_VHF_AM_Volume_Inc:
		axis_value_iterate(VHF_CMD_VOLUME,1);
		break;
	case VHF_CMD_VOLUME :
		communicator.setVolume(value);
		break;

	case iCommandPlane_VHF_AM_Squelch:
	case iCommandPlane_VHF_FM_Squelch:
		switcher_value_iterate(VHF_CMD_SQUELCH);
		break;
	case iCommandPlane_VHF_AM_Tone:
	case iCommandPlane_VHF_FM_Tone:
		button_value_iterate(VHF_CMD_SQUELCH_TONE);
		break;
	case VHF_CMD_SQUELCH :
	case VHF_CMD_SQUELCH_TONE:
		{
			bool isOff = value < -0.5f;
			communicator.setSquelchOnOff(!isOff);
			//TODO: process squelch TONE
			//if (value > 0.5f)
			//	; // transmit 1000Hz tone
		}
		break;

	case iCommandPlane_VHF_FM_Load:
	case iCommandPlane_VHF_AM_Load:
		button_value_iterate(VHF_CMD_LOAD);
		break;
	case VHF_CMD_LOAD :
		if (selectionDial == VHF_FSD_MAN && functional_mode_ == VHF_FMD_TR)
			presetChannels[presetChannel] = getKnobsFrequency();
		break;
	}
}

bool avVHF_ARC_186::checkAnimationConditions(int arg_num, int command, float& value)
{
	switch(command)
	{
	case VHF_CMD_FREQ_SELECTOR_1_INC:
			value = int(value * 20.0f + 0.05f) / 20.0f;
		break;
	case VHF_CMD_FREQ_SELECTOR_2_INC:
	case VHF_CMD_FREQ_SELECTOR_2_DEC:
	case VHF_CMD_FREQ_SELECTOR_3_INC:
	case VHF_CMD_FREQ_SELECTOR_3_DEC:
			value = int(value * 10.0f + 0.5f) / 10.0f;
		break;
	case VHF_CMD_FREQ_SELECTOR_4_INC:
	case VHF_CMD_FREQ_SELECTOR_4_DEC:
		{
			int delta = int(value/0.25 + 0.5);
			value = delta >= 4 ? 0.0f : 0.25f * delta;
		}
		break;
	case VHF_CMD_PRESET_CHANNEL_DEC:
	case VHF_CMD_PRESET_CHANNEL_INC:
		{
			float delta = value * 100.0f;
			value = delta > 0.0f ? int(delta + 0.5f) / 100.0f : int(delta - 0.5f) / 100.0f;
		}
		break;
	default:
			break;
	}
	return true;
}

void avVHF_ARC_186::forceSetArgument(int command, double val)
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

float avVHF_ARC_186::getFrequency() const
{
	selected_frequency_ = getKnobsFrequency();
	float frequency = 0;
	switch (selectionDial)
	{
	case VHF_FSD_EMER_FM :
		if (type_AMFM == VHF_RT_FM)
			frequency = EMER_FM_FQ;
		else
			frequency = selected_frequency_;
		break;

	case VHF_FSD_EMER_AM :
		if (type_AMFM == VHF_RT_AM)
			frequency = EMER_AM_FQ;
		else
			frequency = selected_frequency_;
		break;

	case VHF_FSD_MAN :
		frequency = selected_frequency_;
		break;

	case VHF_FSD_PRE :
		frequency = getPresetFrequency();
		break;
	}
	return frequency;
}

void avVHF_ARC_186::setSelectedFrequency(float frequency)
{
	if(!isAMRadio())
		frequency = MAX(cockpit::MIN_VHF_FM_FREQ, frequency);
	setKnobsFrequency(frequency);

	avVHF_Radio::setSelectedFrequency(frequency);
	onFrequencyChanged();
}

void avVHF_ARC_186::setKnobsFrequency(float frequency)
{
	freq_1st_digit = floor(frequency / 10000000.0f);
	frequency -= freq_1st_digit * 10000000.0f;

	freq_2nd_digit = floor(frequency / 1000000.0f);
	frequency -= freq_2nd_digit * 1000000.0f;

	freq_3rd_digit = floor(frequency /  100000.0f);
	frequency -= freq_3rd_digit * 100000.0f;

	freq_4th_digit = floor(frequency /  1000.0f);
	frequency -= freq_4th_digit * 1000.0f;

	forceSetArgument(VHF_CMD_FREQ_SELECTOR_1_INC, 0.05f * freq_1st_digit);
	forceSetArgument(VHF_CMD_FREQ_SELECTOR_2_INC, 0.1f * freq_2nd_digit);
	forceSetArgument(VHF_CMD_FREQ_SELECTOR_3_INC, 0.1f * freq_3rd_digit);
	forceSetArgument(VHF_CMD_FREQ_SELECTOR_4_INC, 0.01f * freq_4th_digit);
}

float avVHF_ARC_186::getKnobsFrequency() const
{
	return freq_1st_digit * 10000000.0f + freq_2nd_digit * 1000000.0f +
		freq_3rd_digit * 100000.0f + freq_4th_digit * 1000.0f;
}