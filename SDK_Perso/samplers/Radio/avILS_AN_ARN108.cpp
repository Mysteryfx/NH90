#include "stdafx.h"

#include "Avionics/Radio/avILS_AN_ARN108.h"
#include "ccModuleManager.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avILS_AN_ARN108, avILS_AN_ARN108);

int avILS_AN_ARN108::KHZ_translate_matrix[] = {10,15,30,35,50,55,70,75,90,95};

avILS_AN_ARN108::avILS_AN_ARN108()
{
}

avILS_AN_ARN108::~avILS_AN_ARN108()
{
}

void avILS_AN_ARN108::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avILS::initialize(ID,Name,script_name);
	last_KHz_value_ = 0.0;
	last_MHz_value_ = 0.0;
	listen_command_range(this,iCommandPlane_ILS_Power, iCommandPlane_ILS_Volume_Increase);
}

void avILS_AN_ARN108::post_initialize()
{
	avILS::post_initialize();
	performClickableAction(ILS_CMD_VOLUME_UP, 1.0f);
	performClickableAction(ILS_CMD_CHANNEL_SELECTOR_KHZ, 0.9f);
	performClickableAction(ILS_CMD_CHANNEL_SELECTOR_KHZ, 0.95f);
	performClickableAction(ILS_CMD_CHANNEL_SELECTOR_MHZ, 0.0f);
}

void avILS_AN_ARN108::SetCommand(int command, float value )
{
	switch (command)
	{
	case iCommandPlane_ILS_MHz_Decrease:
		axis_value_iterate(ILS_CMD_CHANNEL_SELECTOR_MHZ,-1);
		break;
	case iCommandPlane_ILS_MHz_Increase:
		axis_value_iterate(ILS_CMD_CHANNEL_SELECTOR_MHZ,1);
		break;
	case ILS_CMD_CHANNEL_SELECTOR_MHZ :
		{
			setFrequencyMHz(108 + int(value * 10 + 0.5));
		}
		break;

	case iCommandPlane_ILS_KHz_Decrease:
		axis_value_iterate(ILS_CMD_CHANNEL_SELECTOR_KHZ,-1);
		break;
	case iCommandPlane_ILS_KHz_Increase:
		axis_value_iterate(ILS_CMD_CHANNEL_SELECTOR_KHZ,1);
		break;
	case ILS_CMD_CHANNEL_SELECTOR_KHZ :
		{
			int index = int(value * 10 + 0.5);
			assert(index >= 0 && index <= 9);

			setFrequencyKHz(KHZ_translate_matrix[index] * 10);
		}
		break;

	case iCommandPlane_ILS_Power:
		switcher_value_iterate(ILS_CMD_POWER);
		break;
	case ILS_CMD_POWER :
		setElecPower(value > 0.5);
		break;

	case iCommandPlane_ILS_Volume_Decrease:
		performClickableAction(ILS_CMD_VOLUME_DOWN, volumeLocalizer.getGain()-0.1);
		break;
	case iCommandPlane_ILS_Volume_Increase:
		performClickableAction(ILS_CMD_VOLUME_UP, volumeLocalizer.getGain()+0.1);
		break;
	case ILS_CMD_VOLUME_UP :
	case ILS_CMD_VOLUME_DOWN:
		volumeLocalizer.setGain(value);
		break;
	}
}

bool avILS_AN_ARN108::checkAnimationConditions(int arg_num, int command, float& value)
{
	switch (command)
	{
	case ILS_CMD_CHANNEL_SELECTOR_KHZ:
		{
			int new_value  = 0;
			if (value == 0.0)
			{
				new_value = 9;
			}
			else if (value > last_KHz_value_)
			{
				new_value = int(value*10) + 1;
				if (new_value > 9)
				{
					new_value = 0;
				}
			}
			else
			{
				new_value = int(value*10);
			}
			last_KHz_value_ = value = float(new_value) / 10.0;
		}
		break;
	case ILS_CMD_CHANNEL_SELECTOR_MHZ:
		{
			int new_value  = 0;
			if (value > last_MHz_value_)
			{
				new_value = __min(int(value*10) + 1,3);
			}
			else
			{
				new_value = int(value*10);
			}
			last_MHz_value_ = value = float(new_value) / 10.0;
		}
		break;
	}
	return true;
}

int avILS_AN_ARN108::getFrequencyDigit(int digitPosition)
{
	int digit = 0;
	if (digitPosition == 0) //MHz
	{
		digit = frequencyMHz % 100;
	}
	else if (digitPosition == 1) //KHz
	{
		digit = frequencyKHz / 10;
	}
	return digit;
}