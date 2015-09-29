#pragma once

#include "Avionics/Radio/avILS.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit { 

enum enum_avILS_commands
{
	ILS_CMD_POWER = iCommandCockpitDevice_Action_1,
	ILS_CMD_CHANNEL_SELECTOR_MHZ,
	ILS_CMD_CHANNEL_SELECTOR_KHZ,
	ILS_CMD_VOLUME_UP,
	ILS_CMD_VOLUME_DOWN,
};

class COCKPITBASE_API avILS_AN_ARN108 :  public avILS
{
public:
	avILS_AN_ARN108();
	virtual ~avILS_AN_ARN108();
		
	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	virtual void SetCommand(int command, float value /* = 0 */);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	//end of interface
	int getFrequencyDigit(int digitPosition);
protected:
	double		last_KHz_value_;
	double		last_MHz_value_;
	static int KHZ_translate_matrix[];
private:

};

} 
