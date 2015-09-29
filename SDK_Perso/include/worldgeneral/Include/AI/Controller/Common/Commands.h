#ifndef __Common_Commands__
#define __Common_Commands__

#include "WorldGeneral.h"

#include "AI/Controller/Command.h"
#include "AI/Controller/CommandExecutor.h"
#include "Utilities/Parameter.h"
#include "IwRadioBeacon.h"
enum wModulation;

enum wMsg;

namespace AI
{

BEGIN_AI_COMMAND(NoCommand,			WORLDGENERAL_API)
	virtual void exec(Controller * pController) const;
END_AI_COMMAND

//Runs lua script in Mission Scripting lua-Environment (SSE)
BEGIN_AI_COMMAND(Script,				WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(ed::string, command)
END_AI_COMMAND

//Runs lua script file in Mission Scripting lua-Environment (SSE)
BEGIN_AI_COMMAND(ScriptFile,			WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(ed::string,	file)
END_AI_COMMAND

//Changes callsign of the group or unit
BEGIN_AI_COMMAND(SetCallsign,		WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(bool,		callnameFlag)
	AI_COMMAND_PARAMETER(int,		callname)
	AI_COMMAND_PARAMETER(int,		number)
	AI_COMMAND_PARAMETER(int,		callsign)
END_AI_COMMAND

//Changes frequency and modulation of the group's or unit's radio
BEGIN_AI_COMMAND(SetFrequency,		WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(float,		frequency)
	AI_COMMAND_PARAMETER(wModulation,modulation)
END_AI_COMMAND

//Transmits message from the group's or unit's radio
BEGIN_AI_COMMAND(TransmitMessage,	WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(ed::string,file)
	AI_COMMAND_PARAMETER(ed::string,subtitle)
	AI_COMMAND_PARAMETER(bool,		loop)
	AI_COMMAND_PARAMETER(wModelTime,	duration)
END_AI_COMMAND

//Stops transmission from the group's or unit's radio
BEGIN_AI_COMMAND(StopTransmission,	WORLDGENERAL_API)
END_AI_COMMAND

//Switches current waypoint of the route. Handled by Task State of Mission task.
BEGIN_AI_COMMAND(SwitchWaypoint,		WORLDGENERAL_API)
	virtual void 	exec(Controller * pController_) const;
	AI_COMMAND_PARAMETER(int,		fromWaypointIndex)
	AI_COMMAND_PARAMETER(int,		goToWaypointIndex)
END_AI_COMMAND

//Switches current waypoint of the route. Handled by Task State of Mission task.
BEGIN_AI_COMMAND(StopRoute,			WORLDGENERAL_API)
	virtual void 	exec(Controller * pController_) const;
	AI_COMMAND_PARAMETER(bool,		value)
END_AI_COMMAND

//Switches current action in the actions list. Handled by Task State of subclasses of MultiTask class (Mission, ComboTask, e.t.c).
BEGIN_AI_COMMAND(SwitchAction,		WORLDGENERAL_API)
	virtual void 	exec(Controller * pController_) const;
	AI_COMMAND_PARAMETER(int,		actionIndex)	
END_AI_COMMAND

//Disables "uncontrolled" state for the group and starts the task that is assigned to the group in the Mission Editor
BEGIN_AI_COMMAND(Start,				WORLDGENERAL_API)
END_AI_COMMAND

//Makes the unit/group immortal/vulnerable
BEGIN_AI_COMMAND(SetImmortal,		WORLDGENERAL_API)
	//virtual void 	exec(Controller * pController_) const;
	AI_COMMAND_PARAMETER(bool,			value)
END_AI_COMMAND

//Makes the unit/group invisible/visible
BEGIN_AI_COMMAND(SetInvisible,		WORLDGENERAL_API)
	//virtual void 	exec(Controller * pController_);
	AI_COMMAND_PARAMETER(bool,			value)
END_AI_COMMAND

//Activates radio beacon on board of the unit(s)
BEGIN_AI_COMMAND(ActivateBeacon,		WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(BeaconType,		type)
	AI_COMMAND_PARAMETER(unsigned int,	system)
	AI_COMMAND_PARAMETER(ed::string,	name)
	AI_COMMAND_PARAMETER(ed::string,	callsign)
	AI_COMMAND_PARAMETER(float,			frequency)	
END_AI_COMMAND

//Deactivates radio beacon on board of the unit(s)
BEGIN_AI_COMMAND(DeactivateBeacon,	WORLDGENERAL_API)
END_AI_COMMAND

//Reports message from the unit's or group's radio
BEGIN_AI_COMMAND(Report,				WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(wMsg,			message)
END_AI_COMMAND

//Switches EPLRS on/off
BEGIN_AI_COMMAND(EPLRS,				WORLDGENERAL_API)
	AI_COMMAND_PARAMETER(bool,			value)
	AI_COMMAND_PARAMETER(int,			groupId)
END_AI_COMMAND

//Sets the value of the option
BEGIN_AI_COMMAND(Option,				WORLDGENERAL_API)
	virtual void exec(Controller * pController) const;
	typedef unsigned int OptionName;
	AI_COMMAND_PARAMETER(OptionName,		name)
	typedef Parameter Value;	
	AI_COMMAND_PARAMETER(Value,			value)
END_AI_COMMAND

}

#endif __Common_Commands__