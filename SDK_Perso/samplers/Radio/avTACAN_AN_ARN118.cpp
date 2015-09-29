#include "stdafx.h"

#include "Avionics/Radio/avTACAN_AN_ARN118.h"
#include "ccModuleManager.h"
#include "Utilities/ccUtilities.h"

using namespace cockpit;

const float Y_CHNL = 0.93f;
const float X_CHNL = 0.87f;

avTACAN_AN_ARN118::avTACAN_AN_ARN118()
{
	
}

avTACAN_AN_ARN118::~avTACAN_AN_ARN118()
{
}

void avTACAN_AN_ARN118::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
{
	avTACAN::initialize(ID,Name,script_name);
	channelTensLever = 0;
	channelOnesLever = 0;
	channelXYLever = 0.0;
}

void avTACAN_AN_ARN118::post_initialize()
{
	avTACAN::post_initialize();

	performClickableAction(TACAN_CMD_CHANNEL_XY_MODE, Y_CHNL);
	performClickableAction(TACAN_CMD_CHANNEL_XY_MODE, X_CHNL);

	listen_command_range(this, iCommandPlane_TACAN_Mode_Off, iCommandPlane_TACAN_Test);
}

void avTACAN_AN_ARN118::SetCommand(int command, float value)
{
	switch (command)
	{
	case iCommandPlane_TACAN_Chanel_Tens_Decrease:
		axis_value_iterate(TACAN_CMD_CHANNEL_SELECTOR_TENS_INC,-1);
		break;
	case iCommandPlane_TACAN_Chanel_Tens_Increase:
		axis_value_iterate(TACAN_CMD_CHANNEL_SELECTOR_TENS_INC,1);
		break;
	case TACAN_CMD_CHANNEL_SELECTOR_TENS_INC:
	case TACAN_CMD_CHANNEL_SELECTOR_TENS_DEC:
		{
			channelTensLever += value * 50;
			int channel = int(cockpit::round(channelTensLever)) % 13;
			if (channel < 0)
				channel = 13 + channel;

			setChannelTens(channel);
		}
		break;

	case iCommandPlane_TACAN_Chanel_Ones_Decrease:
		axis_value_iterate(TACAN_CMD_CHANNEL_SELECTOR_ONES_INC, -1);
		break;
	case iCommandPlane_TACAN_Chanel_Ones_Increase:
		axis_value_iterate(TACAN_CMD_CHANNEL_SELECTOR_ONES_INC, 1);
		break;
	case TACAN_CMD_CHANNEL_SELECTOR_ONES_INC:
	case TACAN_CMD_CHANNEL_SELECTOR_ONES_DEC:
		{
			channelOnesLever += value * 10;
			int channel = int(cockpit::round(channelOnesLever)) % 10;
			if (channel < 0)
				channel = 10 + channel;
			setChannelOnes(channel);
		}
		break;
	case iCommandPlane_TACAN_Chanel_XY_Mode:
		switcher_value_iterate(TACAN_CMD_CHANNEL_XY_MODE);
		break;
	case TACAN_CMD_CHANNEL_XY_MODE:
		{
			channelXYLever += value * 10;
			int xy_mode = int(cockpit::round(channelXYLever)) % 10;
			if (xy_mode < 0)
				xy_mode = 10 + xy_mode;				
								
			channelXYMode = (xy_mode % 2);
			onChannelChanged();
		}
		break;

	case iCommandPlane_TACAN_Test:
		button_value_iterate(TACAN_CMD_TEST);
		break;
	case TACAN_CMD_TEST :
		if (!is_test)
			start_test = wTime::GetModelTime();
		is_test = value > 0.1;
		break;

	case iCommandPlane_TACAN_Volume_Decrease:
		axis_value_iterate(TACAN_CMD_VOLUME,-1);
		break;
	case iCommandPlane_TACAN_Volume_Increase:
		axis_value_iterate(TACAN_CMD_VOLUME,1);
		break;
	case TACAN_CMD_VOLUME :
		receiver_.setVolume(value);
		break;

	case iCommandPlane_TACAN_Mode_Off:
		performClickableAction(TACAN_CMD_MODE_DIAL,0.0f);
		break;
	case iCommandPlane_TACAN_Mode_Rec:
		performClickableAction(TACAN_CMD_MODE_DIAL,0.1f);
		break;
	case iCommandPlane_TACAN_Mode_TR:
		performClickableAction(TACAN_CMD_MODE_DIAL,0.2f);
		break;
	case iCommandPlane_TACAN_Mode_AAREC:
		performClickableAction(TACAN_CMD_MODE_DIAL,0.3f);
		break;
	case iCommandPlane_TACAN_Mode_AATR:
		performClickableAction(TACAN_CMD_MODE_DIAL,0.4f);
		break;
	case TACAN_CMD_MODE_DIAL:
		{
			test_light = false;
			int newMode = cockpit::round(value * 10.0);

			if (newMode != modeDial && 
				(modeDial == TACAN_MD_OFF || newMode == TACAN_MD_OFF))
				switch_power(newMode != TACAN_MD_OFF);

			modeDial = newMode;

			onChannelChanged();
		}
		break;
	}
}

bool avTACAN_AN_ARN118::checkAnimationConditions(int arg_num, int command, float& value)
{
	switch (command)
	{
	case TACAN_CMD_CHANNEL_SELECTOR_TENS_INC:
	case TACAN_CMD_CHANNEL_SELECTOR_TENS_DEC:
		{
			double newTensLever = channelTensLever + value * 50;

			if (newTensLever > 12.0 || newTensLever < 0.0)
				return false;
		}
		break;
	case TACAN_CMD_CHANNEL_SELECTOR_ONES_INC:
	case TACAN_CMD_CHANNEL_SELECTOR_ONES_DEC:
		{
			double newOnesLever = channelOnesLever + value * 10;

			if (newOnesLever > 9.0 || newOnesLever < 0.0)
				return false;
		}
		break;
	}

	return true;
}

int avTACAN_AN_ARN118::getChannelDigit(int digitPosition)
{
	int digit = 10;

	switch (digitPosition)
	{
	case 0 :
		digit = channelOnes;
		break;

	case 1 :
		digit = channelTens % 10;
		break;

	case 2 :
		digit = channelTens / 10;
		if (digit == 0)
			digit = 10;
		break;
	}

	return digit;
}