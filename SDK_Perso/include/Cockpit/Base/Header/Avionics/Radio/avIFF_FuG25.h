#pragma once;

#include "Avionics/avDevice.h"
#include "ICommandsCockpitDevice.h"

namespace cockpit
{ 


class COCKPITBASE_API avIFF_FuG25 : public avDevice
{ 
public:
	enum FuG25_commands
	{ 
		CMD_FuG25_MODE = iCommandCockpitDevice_Action_1,
		CMD_FuG25_MODE_EXT,
		CMD_FuG25_MODE_CHECK
	};

public:
	avIFF_FuG25(){}
	virtual ~avIFF_FuG25(){}
	
	//avDevice interface
	virtual void SetCommand(int command,float value);
	//end of interface

protected:
};
}
