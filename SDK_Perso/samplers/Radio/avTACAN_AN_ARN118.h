#pragma once

#include "Avionics/Radio/avTACAN.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit {

enum enum_avTACAN_commands
{
	TACAN_CMD_CHANNEL_SELECTOR_TENS_INC = iCommandCockpitDevice_Action_1,
	TACAN_CMD_CHANNEL_SELECTOR_TENS_DEC,
	TACAN_CMD_CHANNEL_SELECTOR_ONES_INC,
	TACAN_CMD_CHANNEL_SELECTOR_ONES_DEC,
	TACAN_CMD_CHANNEL_XY_MODE,
	TACAN_CMD_TEST,
	TACAN_CMD_VOLUME,
	TACAN_CMD_MODE_DIAL
};

class COCKPITBASE_API avTACAN_AN_ARN118 :  public avTACAN
{
public:
	avTACAN_AN_ARN118();
	virtual ~avTACAN_AN_ARN118();

	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	void SetCommand(int command, float value = 0);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface
	
	// TACAN control panel
	int getChannelDigit(int digitPosition);
	int getChannelXYMode() { return channelXYMode; }

	bool getTestLight() const {return test_light;}

protected:

	double channelTensLever;
	double channelOnesLever;
	double channelXYLever;

protected:
	
};

}
