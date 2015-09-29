#ifndef __AI_Controller__
#define __AI_Controller__

/*
Controller is the object that controls separate AI units or whole groups of AI units.
Controller performs task and commands.
Behavior options can be specified for the controller and they will have effect whatever task it is performing.
*/

#include "WorldGeneral.h"

#include "Utilities/Parameter.h"
#include "cAlgorithm.h"

#include <stack>
#include <ed/string.h>

class MovingObject;
struct lua_State;

namespace Lua
{
	class Config;
}

namespace AI
{

class Command;
class Task;
class TaskState;
enum Event;

class ControllerScheduledAction;

class WORLDGENERAL_API Controller
{
protected:
	typedef Parameter				Option;
	typedef unsigned int			OptionName;
public:
	enum
	{
		NO_OPTION = UINT_MAX,
		OPTION_MAX = 26,
	};
	struct Descriptor
	{
		Descriptor(const char * name_, Descriptor * pParent_) : name(name_), pParent(pParent_) {;}
		const char * name;
		Descriptor * pParent;
	};
	Controller();
	virtual ~Controller();
	virtual void			setOnOff(bool onOff); //Switches controller on/off
	virtual Descriptor *	getDesc() const; //Returns the controller's descriptor
	void					setCommand(const Command & command); //Copies the command and does it
	virtual void			setCommand(Command * command); //Does the command using shared_ptr
	TaskState*				setTask(const Task & task); //Copies the task and sets it
	virtual TaskState*		setTask(Task * task); //Sets the task using shared_ptr
	TaskState *				pushTask(const Task & task); //Copies the task and pushes it
	virtual TaskState*		pushTask(Task * task);//Pushes the task using shared_ptr
	virtual bool			resetTask(); //Resets current task
	virtual void			onEvent(Event event, const Parameter * parameters = NULL); //Processes event
	void					update(); //update
	inline bool				hasTask() const { return pTaskState_ != NULL; }
	inline TaskState*		getTaskState() const { return pTaskState_; }
	inline const Task*		getTask() const;
	virtual void			setOption(OptionName name, const Option & value); //Sets the option
	virtual const Option &	getOption(OptionName name) const; //Returns the value of the given option 
protected:
	Option					options[OPTION_MAX]; //Option values
private:
	TaskState *				pTaskState_;
	typedef ControllerScheduledAction ScheduledAction;
	std::stack<ScheduledAction*> scheduledActions_; //Delayed task or command operation
	bool					inRun_;
};

WORLDGENERAL_API Task * loadTask(Lua::Config & config, ed::string & errorMsg);

WORLDGENERAL_API Command * loadCommand(Lua::Config & config, ed::string & errorMsg);

#define GET_DESC_METHOD(ControllerType, ControllerParentType) virtual AI::Controller::Descriptor * getDesc() const\
{\
	static bool guard = false;\
	assert(!guard);\
	guard = true;\
	static AI::Controller::Descriptor desc(SYMBOL_STR(ControllerType), (this)->ControllerParentType::getDesc() );\
	guard = false;\
	return &desc;\
}

}

#endif __AI_Controller__