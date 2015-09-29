#include "stdafx.h"

#include "Avionics/Radio/avIFF_APX_72.h"

using namespace cockpit;

void avIFF_APX_72::post_initialize()
{
    current_code_position_ = IFF_CODE_Zero;

	lamps_[IFF_TEST_LAMP].brightness_ = 1.0;
	lamps_[IFF_REPLY_LAMP].brightness_ = 1.0;

	power_switch_ >> reply_switch_ >> reply_press_ >> reply_nodeor_ >> lamps_[IFF_REPLY_LAMP].lamp_consumer_;
	power_switch_ >> reply_test_switch_ >> reply_test_press_ >> reply_nodeor_;

	power_switch_ >> test_switch_ >> lamps_[IFF_TEST_LAMP].lamp_consumer_;
	reply_press_.setOnOff(true);
	reply_test_switch_.setOnOff(true);

	performClickableAction(Command_IFF_Reply_Light_Dimming,1.0f);
	performClickableAction(Command_IFF_Test_Light_Dimming,1.0f);
	listen_command_range(this, iCommandIFFAntennaUpper, iCommandIFFAntennaLower);
}

bool avIFF_APX_72::checkAnimationConditions(int arg_num, int command, float& value)
{
	if(command == Command_IFF_Code)
	{
		if (value < -0.5)
		{
			if (current_code_position_ < IFF_CODE_HOLD)
			{
				value = (++current_code_position_) / 10.0;
				return true;
			}
		}
		else if (value > 0.5)
		{
			if (current_code_position_ > IFF_CODE_Zero)
			{
				value = (--current_code_position_) / 10.0;
				return true;
			}
		}
		else
		{
			if (current_code_position_ == IFF_CODE_HOLD)
			{
				value = (--current_code_position_) / 10.0;
				return true;
			}
		}
		return false;
	}
	return true;
}

void avIFF_APX_72::SetCommand( int command,float value )
{
	switch (command)
    { 
    case Command_IFF_Mode1_Wheel_1:
        break;
    case Command_IFF_Mode1_Wheel_2:
        break;
    case Command_IFF_Mode3A_Wheel_1:
        break;
    case Command_IFF_Mode3A_Wheel_2:
        break;
	case Command_IFF_Mode3A_Wheel_3:
        break;
    case Command_IFF_Mode3A_Wheel_4:
        break;
	case Command_IFF_Master:
		power_switch_.setOnOff(value > 0.05);
		break;
	case Command_IFF_Audio_Light:
		if (value > 0.5 || value < -0.5)
		{
			reply_switch_.setOnOff(true);
			reply_test_switch_.setOnOff(false);
		}
		else
		{
			reply_switch_.setOnOff(false);
			reply_test_switch_.setOnOff(true);
		}
		break;
	case Command_IFF_Test_M_1:
		test_switch_.setOnOff(value > 0.5);
		break;
	case Command_IFF_Test_M_2:
		test_switch_.setOnOff(value > 0.5);
		break;
	case Command_IFF_Test_M_3A:
		test_switch_.setOnOff(value > 0.5);
		break;
	case Command_IFF_Test_M_C:
		test_switch_.setOnOff(value > 0.5);
		break;
	case Command_IFF_RAD_Test_Mon:
		break;
	case Command_IFF_Mic_Ident:
		break;
	case Command_IFF_On_Off:
		break;
	case Command_IFF_Reply:
		reply_press_.setOnOff(value < 0.5);
		reply_test_press_.setOnOff(value > 0.5);		
		break;
	case Command_IFF_Test:
		test_switch_.setOnOff(value > 0.5);
		break;
	case iCommandIFFAntennaLower:
		performClickableAction(Command_IFF_Antenna,0.0f);
		break;
	case iCommandIFFAntennaBoth:
		performClickableAction(Command_IFF_Antenna,0.5f);
		break;
	case iCommandIFFAntennaUpper:
		performClickableAction(Command_IFF_Antenna,1.0f);
		break;
	case Command_IFF_Antenna:
		break;
	case Command_IFF_Reply_Light_Dimming:
		lamps_[IFF_REPLY_LAMP].brightness_ = value;
		break;
	case Command_IFF_Test_Light_Dimming:
		lamps_[IFF_TEST_LAMP].brightness_ = value;
		break;
	default:
		break;
    }
}

double avIFF_APX_72::get_light_status(unsigned int lamp_id) const 
{
	assert(lamp_id < IFF_LAMPS_MAX);
	if (lamp_id >= IFF_LAMPS_MAX)
	{
		return 0.0;
	}
	return lamps_[lamp_id].brightness_* ( lamps_[lamp_id].lamp_consumer_.GetVoltage() > 1.0);
}
