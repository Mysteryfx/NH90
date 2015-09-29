#ifndef __CommandExecutor__
#define __CommandExecutor__

//Command Executor is a singleton object that performs Command for the Controller.
//Type of Command Executor is depended on the Command type and the Controller type.
//Command Executor is constant object and its state can't be changed.

#include "AI/Controller/Storage.h"
#include "AI/Utilities/Other.h"

namespace Lua
{
class Config;
}

namespace AI
{

class Command;
class Controller;

class CommandExecutor
{
public:
	virtual void exec(const Command * pCommand_, Controller * pController_) const = 0;
};

//Storage of Command Executors
typedef const char * CommandExecutorId;
template Storage<CommandExecutorId, CommandExecutor*, stdext::hash_compare<CommandExecutorId, cstr_less>>;
typedef Storage<CommandExecutorId, CommandExecutor*, stdext::hash_compare<CommandExecutorId, cstr_less>>	CommandExecutors;

#define GET_COMMAND_EXECUTOR_CLASS_NAME_STR(ControllerClass, CommandClass) #ControllerClass#CommandClass
#define GET_COMMAND_EXECUTOR_CLASS(ControllerClass, CommandClass) ControllerClass##CommandClass

#define AI_COMMAND_EXECUTOR(CommandTypeIn, ControllerTypeIn, ...)  class __VA_ARGS__ GET_COMMAND_EXECUTOR_CLASS(ControllerTypeIn, CommandTypeIn) : public CommandExecutor\
{\
private:\
	typedef CommandTypeIn CommandType;\
	typedef ControllerTypeIn ControllerType;\
	typedef GET_COMMAND_EXECUTOR_CLASS(ControllerTypeIn, CommandTypeIn) SelfType;\
public:\
	SelfType();\
	inline static void reg() { CommandExecutors::instance()->add(GET_COMMAND_EXECUTOR_CLASS_NAME_STR(ControllerTypeIn, CommandTypeIn), new SelfType()); }\
	virtual void exec(const Command * pCommand_, Controller * pController_) const\
	{\
		typedExec_(static_cast<const CommandType*>(pCommand_), static_cast<ControllerType*>(pController_));\
	}\
protected:\
	virtual void typedExec_(const CommandType * p##CommandTypeIn, ControllerType * p##ControllerTypeIn) const;\
};

#define REGISTER_AI_COMMAND_EXECUTOR(ControllerClass, CommandClass) ControllerClass##CommandClass::reg();

}

#endif __CommandExecutor__