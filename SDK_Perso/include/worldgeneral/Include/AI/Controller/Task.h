#ifndef __Task__
#define __Task__

//Task is a set of parameters of the process that the controller must perform.
//Controller receives the task and starts performing it.
//Controller never changes the task received from outside.

#include "WorldGeneral.h"

#include "AI/Controller/ClassFactory.h"
#include "AI/Utilities/Other.h"

#include <ed/vector.h>
#include <ed/list.h>

#include <boost/smart_ptr/intrusive_ptr.hpp>

typedef double wModelTime;

namespace Lua
{
	class Config;
}

class MovingObject;

namespace AI
{

class Controller;
class TargetFilter;
class Task;
class TaskState;
class TaskConstVisitor;
class TaskVisitor;

class WORLDGENERAL_API Task
{
public:
	enum {
		CATEGORY_VOID		= 0,
		CATEGORY_NAV		= 1 << 0,
		CATEGORY_ATTACK		= 1 << 1,
		CATEGORY_PROTECT	= 1 << 2,
		CATEGORY_RECON		= 1 << 3,
		CATEGORY_COMMAND	= 1 << 4,	
		CATEGORY_SUPPLY		= 1 << 5,
		CATEGORY_COMBAT		= CATEGORY_ATTACK | CATEGORY_PROTECT | CATEGORY_COMMAND | CATEGORY_RECON
	};
	typedef unsigned int Category;
	typedef Task*(*Creator)();
	Task();
	virtual ~Task();
	virtual Task *			clone() const = 0;
	virtual void			load(Lua::Config & config) = 0;
	virtual TaskState *		exec(Controller * pController);
	virtual bool			visit(TaskConstVisitor * taskVisitor, unsigned int level = 0) const;
	virtual bool			visit(TaskVisitor * taskVisitor, unsigned int level = 0);
	virtual const char *	getName() const = 0;
	virtual bool			isBackGround() const = 0;
	virtual Category		getCategory() const = 0;
#ifdef DEBUG_TEXT
	virtual const char *	print(char * buff, int size) const;
#endif
	static ed::list<Task*> allTasks;
	unsigned int refCount;
};

//boost::intrusive_ptr for Task

inline void intrusive_ptr_add_ref(Task * pTask)
{
	++pTask->refCount;
}

inline void intrusive_ptr_release(Task * pTask)
{
	if(--pTask->refCount == 0)
		delete pTask;
}

typedef boost::intrusive_ptr<Task> TaskPtr;
typedef boost::intrusive_ptr<const Task> TaskConstPtr;
typedef ed::vector<TaskPtr> Tasks;

//Task factory
typedef const char * TaskId;
template WORLDGENERAL_API ClassFactory<TaskId, Task*, stdext::hash_compare<TaskId, cstr_less>>;
typedef ClassFactory<TaskId, Task*, stdext::hash_compare<TaskId, cstr_less>>	TaskFactory;

#define REGISTER_AI_TASK(TaskType) TaskFactory::instance()->addCreator(#TaskType, &TaskType::create);

//Main task. Describes the solid process. Main task has start and finish.

class MainTaskState;

class WORLDGENERAL_API MainTask : public Task
{
public:
	typedef MainTaskState State;
	virtual bool			isBackGround() const;
};

//Background task is just an addition to current top main task. Finished only when current top main task is finished.

class BackGroundTaskState;

class WORLDGENERAL_API BackGroundTask : public Task
{
public:
	typedef BackGroundTaskState State;
	virtual bool			isBackGround() const;
};

//Flexible task can be main task or background task as well. It is just a decorator for the task.

class FlexyTaskState;

class WORLDGENERAL_API FlexyTask : public Task
{
public:
	FlexyTask();
	typedef FlexyTaskState State;
	void					load(Lua::Config & config);
	virtual bool			visit(TaskConstVisitor * taskVisitor, unsigned int level = 0) const;
	virtual bool			visit(TaskVisitor * taskVisitor, unsigned int level = 0);
	virtual bool			isBackGround() const;
	virtual Category		getCategory() const;
	TaskPtr 				m_pTask;
};

//Multi task is set of several main tasks which are must being performing in defined order.
//Multi task can be a list of tasks, stack of tasks, hierarchical or network combination of tasks.

class MultiTaskState;

class WORLDGENERAL_API MultiTask : public Task
{
public:
	typedef MultiTaskState State;
	MultiTask();
	MultiTask(const MultiTask & multiTask);
	MultiTask & operator = (const MultiTask & multiTask);
	~MultiTask();
	virtual bool			visit(TaskConstVisitor * taskVisitor, unsigned int level = 0) const;
	virtual bool			visit(TaskVisitor * taskVisitor, unsigned int level = 0);
	virtual bool			isBackGround() const;
	virtual Category		getCategory() const;
	Tasks					tasks;
};

#define BEGIN_AI_TASK_(TaskType, BaseTaskType, ...) class __VA_ARGS__ TaskType : public BaseTaskType\
{\
public:\
	typedef BaseTaskType ParentType;\
	TaskType();\
	virtual TaskType *		clone() const { return new TaskType(*this); }\
	virtual void			load(Lua::Config & config);\
	static Task *			create() { return new TaskType(); }\
	virtual const char *	getName() const { return #TaskType; }
#define BEGIN_AI_TASK_COMMON(TaskType, BaseType, ...)			BEGIN_AI_TASK_(TaskType, BaseType,			##__VA_ARGS__)
#define BEGIN_AI_TASK(TaskType, category, ...)					BEGIN_AI_TASK_(TaskType, MainTask,			##__VA_ARGS__)\
	virtual Category getCategory() const { return category; }
#define BEGIN_AI_BACKGROUND_TASK(TaskType, category, ...)		BEGIN_AI_TASK_(TaskType, BackGroundTask,	##__VA_ARGS__)\
	virtual Category getCategory() const { return category; }
#define BEGIN_AI_FLEXY_TASK(TaskType, ...)						BEGIN_AI_TASK_(TaskType, FlexyTask,			##__VA_ARGS__)
#define BEGIN_AI_MULTI_TASK(TaskType, ...)						BEGIN_AI_TASK_(TaskType, MultiTask,			##__VA_ARGS__)
#define AI_TASK_PARAMETER(ParamType, ParamName) ParamType ParamName;
#define END_AI_TASK };

#define AI_TASK_CREATE virtual TaskState * exec(Controller * pController_);
#define AI_TASK_VOID_CREATE virtual TaskState * exec(Controller * pController_) { return NULL; }

//Task visitors

class WORLDGENERAL_API TaskVisitor
{
public:
	~TaskVisitor();
	virtual bool operator()(Task * task, unsigned int level) = 0;
};

class WORLDGENERAL_API TaskConstVisitor
{
public:
	~TaskConstVisitor();
	virtual bool operator()(const Task * task, unsigned int level) = 0;
};

}

#endif __Task__