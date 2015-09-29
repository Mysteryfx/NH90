#ifndef __Command__
#define __Command__

/*
Command is just a single action that the Controller must perform.
Command is not a process so it requires no time to perform.
*/

#include "WorldGeneral.h"

#include "AI/Controller/ClassFactory.h"
#include "AI/Utilities/Other.h"

#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace Lua
{
	class Config;
}

namespace AI
{

class Controller;

class WORLDGENERAL_API Command
{
public:
	typedef Command*(*Creator)();
	Command();
	virtual ~Command();
	virtual Command *		clone() const = 0;
	virtual void			load(Lua::Config & config) = 0;
	virtual void 			exec(Controller * pController_) const;
	virtual const char *	getName() const = 0;
	unsigned int refCount;
};

//boost::intrusive_ptr for Command

inline void intrusive_ptr_add_ref(Command * pCommand)
{
	++pCommand->refCount;
}

inline void intrusive_ptr_release(Command * pCommand)
{
	if(--pCommand->refCount == 0)
		delete pCommand;
}

typedef boost::intrusive_ptr<Command> CommandPtr;
typedef boost::intrusive_ptr<const Command> CommandConstPtr;

//Command factory

typedef const char * CommandId;
template ClassFactory<CommandId, Command*, stdext::hash_compare<CommandId, cstr_less>>;
typedef ClassFactory<CommandId, Command*, stdext::hash_compare<CommandId, cstr_less>>	CommandFactory;

#define REGISTER_AI_COMMAND(CommandType) CommandFactory::instance()->addCreator(#CommandType, &CommandType::create);

#define BEGIN_AI_COMMAND(CommandType, ...) class __VA_ARGS__ CommandType : public Command\
{\
public:\
	CommandType();\
	virtual CommandType * clone() const { return new CommandType(*this); }\
	virtual void load(Lua::Config & config);\
	static Command * create() { return new CommandType(); }\
	virtual const char * getName() const { return #CommandType; }
#define AI_COMMAND_PARAMETER(ParamType, ParamName) ParamType ParamName;
#define END_AI_COMMAND };

}

#endif __Command__