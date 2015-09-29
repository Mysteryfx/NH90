#ifndef __TaskState__
#define __TaskState__

//Task State is current state of the task performing.
//Type of Task State is depended on the Task type and the Controller type.
//Task State is created by Controller when it receives Task.
//Task is a set of parameters for Task State.
//Task State is changed by Controller.

#include "WorldGeneral.h"

#include "AI/Controller/Task.h"
#include "AI/Controller/Storage.h"
#include "AI/Utilities/Other.h"
#include "AI/Utilities/TargetFilters.h"

#include "Utilities/Parameter.h"

#include <deque>
#include <memory>

typedef double wModelTime;

namespace Lua
{
	class Config;
}

class MovingObject;

namespace AI
{

enum Event;
class TaskStateConstVisitor;
class TaskStateVisitor;

class WORLDGENERAL_API TaskState
{
public:
	typedef Task::Category Category;
	enum State
	{
		STATE_FINISHED,
		STATE_SUSPENDED,
		STATE_ACTIVE
	};
	typedef TaskState* (*Creator)(Task * taskIn, Controller * pControllerIn);
	TaskState();
	virtual ~TaskState() = 0;
	virtual TaskState*	clone() = 0;
	virtual bool		start();
	virtual void		update() = 0;
	void				pause(); //Common + task-specific code
	void				resume(); //Common + task-specific code
	virtual void		finish();
	void				visitAsync(TaskStateVisitor * taskStateVisitor); //Send visitor to call it when TaskState::update() is finished
	virtual bool		visit(TaskStateConstVisitor * taskStateVisitor, unsigned int level) const; //Send visitor to call it now
	virtual bool		visit(TaskStateVisitor * taskStateVisitor, unsigned int level); //Send visitor to call it now
	virtual void		onEvent(Event event, const Parameter * parameters = NULL);
	inline State		getState() const { return state_; }
	inline bool			isFinished() const { return state_ == STATE_FINISHED; }
	virtual Task *		getTask() const = 0;
	virtual Controller* getController() const = 0;
	virtual bool		isBackGround() const = 0;
	virtual Category	getCategory() const;
	static ed::list<TaskState*> allTaskStates;
protected:
	virtual void		doPause();  //Task-specific code, please overridde this funciton, not pause()
	virtual void		doResume(); //Task-specific code, please overridde this funciton, not resume()
	class VisitorStack
	{
	public:
		VisitorStack();
		~VisitorStack();
		void push(TaskStateVisitor * taskStateVisitor);
		void process(TaskState * pTaskState);
		void clear();
		inline bool empty() const { return stack_.empty(); }
	private:
		typedef ed::deque<TaskStateVisitor*> Stack;
		Stack stack_;
	};
	VisitorStack	visitorStack;
	friend class	VisitorStack;
	void			processVisitors();
protected:		
	State			state_;
};

class WORLDGENERAL_API MainTaskState : public TaskState
{
public:
	virtual void update();
	virtual bool isBackGround() const;
protected:
	virtual bool checkFinish() = 0;
};

class WORLDGENERAL_API BackGroundTaskState : public TaskState
{
public:
	virtual bool isBackGround() const;
};

class WORLDGENERAL_API FlexyTaskState : public TaskState
{
public:
	FlexyTaskState(FlexyTask * taskIn, Controller * controllerIn);
	FlexyTaskState(const FlexyTaskState & flexyTaskState);
	~FlexyTaskState();
	virtual bool		start();
	virtual void		finish();
	virtual bool		visit(TaskStateConstVisitor * taskStateVisitor, unsigned int level) const;
	virtual bool		visit(TaskStateVisitor * taskStateVisitor, unsigned int level);
	virtual void		onEvent(Event event, const Parameter * parameters = NULL);
	virtual FlexyTask * getTask() const = 0;
	virtual bool		isBackGround() const;
	virtual Category	getCategory() const;
protected:
	virtual void		doPause();
	virtual void		doResume();
	TaskState *			pTaskState;
};

class WORLDGENERAL_API MultiTaskState : public TaskState
{
public:
typedef ed::vector<TaskState*>	TaskStates;
public:
	MultiTaskState();
	~MultiTaskState();
	virtual void	finish();
	virtual bool	visit(TaskStateConstVisitor * taskStateVisitor, unsigned int level) const;
	virtual bool	visit(TaskStateVisitor * taskStateVisitor, unsigned int level);
	virtual void	onEvent(Event event, const Parameter * parameters = NULL);
	virtual MultiTask * getTask() const = 0;
	virtual Category getCategory() const;
    virtual TaskStates getTaskStates() const;

protected:
	void			generateEvent(Event event, const Parameter * parameters = NULL);
	void			onTaskStart_(TaskState * pTaskState, size_t index);
	void			onTaskStart2_(TaskState * pTaskState, size_t minIndex);
	void			onTaskFinish_(size_t index);
	void			onTaskFinish_(TaskState * pTaskState);
	TaskStates		taskStates;
};

//Storage of Task State constructors
typedef const char * TaskStateId;
template Storage<TaskStateId, TaskState::Creator, stdext::hash_compare<TaskStateId, cstr_less>>;
typedef Storage<TaskStateId, TaskState::Creator, stdext::hash_compare<TaskStateId, cstr_less>>	TaskStateCreators;

#define GET_TASK_STATE_CLASS_NAME_STR(ControllerClass, TaskClass) #ControllerClass#TaskClass
#define TASK_STATE_CLASS(ControllerClass, TaskClass) ControllerClass##TaskClass

#define AI_TASK_STATE_BEGIN_(TaskTypeIn, ParentTaskType, ControllerTypeIn, ...) class __VA_ARGS__ TASK_STATE_CLASS(ControllerTypeIn, TaskTypeIn) : public TaskTypeIn::State\
{\
	typedef ParentTaskType::State ParentType;\
public:\
	typedef TaskTypeIn TaskType;\
	virtual bool start();\
	virtual void update();\
	virtual void finish();\
	virtual TaskType * getTask() const { return task.get(); }\
protected:\
	boost::intrusive_ptr<TaskType> task;

#define AI_TASK_STATE_BEGIN(TaskTypeIn, ControllerTypeIn, ...) AI_TASK_STATE_BEGIN_(TaskTypeIn, MainTask, ControllerTypeIn, ##__VA_ARGS__)\
protected:

#define AI_BACKGROUND_TASK_STATE_BEGIN(TaskTypeIn, ControllerTypeIn, ...) AI_TASK_STATE_BEGIN_(TaskTypeIn, BackGroundTask, ControllerTypeIn, ##__VA_ARGS__)\
protected:

#define AI_FLEXY_TASK_STATE_BEGIN(TaskTypeIn, ControllerTypeIn, ...) AI_TASK_STATE_BEGIN_(TaskTypeIn, FlexyTask, ControllerTypeIn, ##__VA_ARGS__)\
protected:

#define AI_MULTI_TASK_STATE_BEGIN(TaskTypeIn, ControllerTypeIn, ...) AI_TASK_STATE_BEGIN_(TaskTypeIn, MultiTask, ControllerTypeIn, ##__VA_ARGS__)\
protected:

#define AI_TASK_STATE_END };

#define AI_TASK_STATE_ON_EVENT virtual void	onEvent(Event event, const Parameter * parameters = NULL);

#define AI_TASK_STATE_FINAL_(TaskTypeIn, ControllerTypeIn) public:\
	typedef ControllerTypeIn ControllerType;\
	typedef TASK_STATE_CLASS(ControllerTypeIn, TaskTypeIn) SelfType;\
	SelfType(TaskType * taskIn, ControllerType * pControllerIn);\
	virtual TaskState * clone() { return new SelfType(*this); }\
	static TaskState * create(Task * taskIn, Controller * pControllerIn) { return new SelfType(static_cast<TaskType*>(taskIn), static_cast<ControllerType*>(pControllerIn)); }\
	inline static void reg() { TaskStateCreators::instance()->add(GET_TASK_STATE_CLASS_NAME_STR(ControllerTypeIn, TaskTypeIn), &SelfType::create); }\
	virtual Controller * getController() const { return pController; }\
protected:\
	ControllerType * const pController;

#define AI_TASK_STATE_FINAL(TaskTypeIn, ControllerTypeIn) AI_TASK_STATE_FINAL_(TaskTypeIn, ControllerTypeIn)

#define AI_TASK_STATE_BASE(TaskTypeIn, ControllerTypeIn) TASK_STATE_CLASS(ControllerTypeIn, TaskTypeIn)(TaskType * task_);

#define AI_TASK_STATE_DERIV_BEGIN(TaskTypeIn, ControllerTypeIn, BaseControllerType) class TASK_STATE_CLASS(ControllerTypeIn, TaskTypeIn) : public TASK_STATE_CLASS(BaseControllerType, TaskTypeIn)\
	{\
		typedef TASK_STATE_CLASS(BaseControllerType, TaskTypeIn) ParentType;\
		public:

#define AI_TASK_STATE_FINAL_DERIV_BEGIN(TaskTypeIn, ControllerTypeIn, BaseControllerType) AI_TASK_STATE_DERIV_BEGIN(TaskTypeIn, ControllerTypeIn, BaseControllerType)\
	AI_TASK_STATE_FINAL_(TaskTypeIn, ControllerTypeIn)

#define REGISTER_AI_TASK_STATE(ControllerClass, TaskClass) TASK_STATE_CLASS(ControllerClass, TaskClass)::reg();

//Task State visitors

class WORLDGENERAL_API TaskStateVisitor
{
public:
	~TaskStateVisitor();
	virtual bool operator()(TaskState * taskState, unsigned int level) = 0;
};

class WORLDGENERAL_API TaskStateConstVisitor
{
public:
	~TaskStateConstVisitor();
	virtual bool operator()(const TaskState * taskState, unsigned int level) = 0;
};

}

#endif __TaskState__