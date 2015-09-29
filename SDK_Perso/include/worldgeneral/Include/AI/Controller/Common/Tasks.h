#ifndef __Common_Tasks__
#define __Common_Tasks__

//Most used tasks

#include "WorldGeneral.h"

#include "AI/Controller/TaskState.h"
#include "AI/Controller/Condition.h"
#include "AI/Controller/Command.h"
#include "AI/Controller/Mission.h"

#include <ed/list.h>
#include "cLinear.h"
#include "wControl.h"


enum wModulation;




namespace AI
{

typedef ed::list<ed::string> TargetTypes;

void loadTargetTypes(Lua::Config & config, TargetTypes & targetTypes);
	
BEGIN_AI_TASK(NoTask, CATEGORY_VOID, WORLDGENERAL_API)
	AI_TASK_VOID_CREATE
END_AI_TASK

//Adapter for the Command to the Task
BEGIN_AI_TASK(WrappedAction, CATEGORY_VOID, WORLDGENERAL_API)
	virtual TaskState * exec(Controller * pController_);
	AI_TASK_PARAMETER(CommandPtr,		pCommand)
END_AI_TASK

//Decorator for the Task that adds start and stop conditions which can be defined in the Mission Editor
BEGIN_AI_FLEXY_TASK(ControlledTask, WORLDGENERAL_API)
	virtual TaskState * exec(Controller * pController_);
	AI_TASK_PARAMETER(StartCondition,	startCondition)
	AI_TASK_PARAMETER(StopCondition,	stopCondition)
END_AI_TASK

//Task State of ControlledTask
AI_FLEXY_TASK_STATE_BEGIN(ControlledTask, Controller)
	AI_TASK_STATE_FINAL(ControlledTask, Controller)
	void			onEvent(Event event, const Parameter * parameters);
	wModelTime		taskStartTime;
	int				fromWaypoint;
AI_TASK_STATE_END

//List of the task. Tasks are performing one-by-one
BEGIN_AI_MULTI_TASK(ComboTask, WORLDGENERAL_API)
END_AI_TASK

//Task State of the ComboTask task
AI_MULTI_TASK_STATE_BEGIN(ComboTask, Controller)
public:
	void stopMainTask();
	void switchAction(unsigned int actionIndex);
AI_TASK_STATE_FINAL(ComboTask, Controller)
	virtual bool isBackGround() const;
	void startSubTask_(Task * pTask);
	void finishSubTask_(TaskState * & pTaskState);
	void onFinishSubTask_(TaskState * & pTaskState);
	void goNextSubTask_();
	void switchSubTask_(int taskIndex);
	unsigned int						currentTaskIndex;
	TaskState*							pMainTaskState;

	typedef std::pair<Task*, TaskState*>  TaskPair;
	typedef ed::vector<TaskPair>	      BackGroundTaskStates;
	BackGroundTaskStates				  backGroundTaskStates;
    ed::vector<Task*>                     m_arNoStartedTasks;
AI_TASK_STATE_END

//Dynamic stack of tasks
//Task list 
BEGIN_AI_MULTI_TASK(Stack, WORLDGENERAL_API)
	AI_TASK_PARAMETER(TaskState*,	prevTaskState)
END_AI_TASK

//Task State of Stack task
AI_MULTI_TASK_STATE_BEGIN(Stack,			Controller, WORLDGENERAL_API)
public:
	void pushTaskFront(Task * pTask);
	void pushTaskBack(Task * pTask);
	void addBackGroundTask(BackGroundTask * pTask);
	void popTask();
    bool moving(bool bMove);
AI_TASK_STATE_FINAL(Stack, Controller)
	virtual bool isBackGround() const;
	void pushFront_(Task * pTask);
	void pushBack_(Task * pTask);
	void pop_();
	void next_();
	void addBackGroundTask_(BackGroundTask * pBackGroundTask);
	struct RunningTask
	{
		RunningTask(Task * pTaskIn) : pTask(pTaskIn), pTaskState(NULL) {;}
		RunningTask(TaskState * pTaskStateIn) : pTask(pTaskStateIn->getTask()), pTaskState(pTaskStateIn) {;}
		bool		run(Controller * pController);
		void		stop();
		void		clear();
		inline Category	getCategory() const { return pTaskState != NULL ? pTaskState->getCategory() : Task::CATEGORY_VOID; }
		inline bool		isBackGround() const { return pTaskState != NULL ? pTaskState->isBackGround() : pTask->isBackGround(); }
		Task *	pTask;
		TaskState *		pTaskState;
	};
	typedef ed::deque<RunningTask> RunningTasksStack;
	RunningTasksStack runningTasksStack;
	typedef ed::list<TaskState*> RunningBackGroundTasks;
	RunningBackGroundTasks runningBackGroundTasks;
AI_TASK_STATE_END

//Attack of the known unit with attack parameters
BEGIN_AI_TASK(AttackUnit,				Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string,		unitId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)	
END_AI_TASK

//Attack of the known group with attack parameters
BEGIN_AI_TASK(AttackGroup,				Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(unsigned int,		groupId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
END_AI_TASK

//Search and then attack of nearest target to the given point within the given distance. Flight altitude and velocity are also parameters
BEGIN_AI_TASK(FindAndAttackTargetAtPoint,		Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(float,			maxDistance)
	AI_TASK_PARAMETER(TargetTypes,		targetTypes)
	AI_TASK_PARAMETER(bool,				group)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
	AI_TASK_PARAMETER(float,			velocity)
	AI_TASK_PARAMETER(float,			altitude)
	AI_TASK_PARAMETER(bool,				ralt)
END_AI_TASK

//Attack of targets of the given types around the group/unit. Attack parameters are provided
BEGIN_AI_TASK(AttackTargetsAround,		Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(float,			maxDistance)
	AI_TASK_PARAMETER(TargetTypes,		targetTypes)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
END_AI_TASK

//Attack of targets of the given types in the given circle zone. Attack parameters are provided
BEGIN_AI_TASK(AttackTargetsInZone,		Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(float,			radius)
	AI_TASK_PARAMETER(TargetTypes,		targetTypes)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
END_AI_TASK

//Attack of closest map object to the given point. Attack parameters are provided
BEGIN_AI_TASK(AttackMapObject,			Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)	
	AI_TASK_PARAMETER(AttackParam,		attackParam)	
END_AI_TASK

//Bombing the given point. Attack parameters are provided
BEGIN_AI_TASK(Bombing,					Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(AttackParam,		attackParam)	
END_AI_TASK

//Bombing the given runway. Attack parameters are provided
BEGIN_AI_TASK(BombingRunway,			Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(int,				runwayId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
END_AI_TASK

enum DesignationMethod { DESIGNATION_NO, DESIGNATION_AUTO, DESIGNATION_WP, DESIGNATION_IR_POINTER, DESIGNATION_LASER };

//Make the unit/group a FAC and order it to assign the enemy group to the friendly CAS flight. Attack parameters are provided
BEGIN_AI_TASK(FAC_AttackGroup,			Task::CATEGORY_COMMAND | Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(unsigned int,		groupId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
	AI_TASK_PARAMETER(DesignationMethod,designationMethod)
	AI_TASK_PARAMETER(bool,				useDatalink)
	AI_TASK_PARAMETER(int,				nCallsign)
	AI_TASK_PARAMETER(float,		    fFrequency)
	AI_TASK_PARAMETER(wModulation,		modulation)
END_AI_TASK

//Fly orbit: race-track or circle
BEGIN_AI_TASK(Orbit,					Task::CATEGORY_NAV, WORLDGENERAL_API)
	enum Pattern { PATTERN_NO, PATTERN_ORBIT_AROUND_WAYPOINT, PATTERN_BETWEEN_TWO_WAYPOINTS };
	enum Flags {
		FLAG_ORBIT_POINT = 1 << 0,
		FLAG_ORBIT_SPEED = 1 << 1,
		FLAG_ORBIT_ALTITUDE = 1 << 2,
		FLAG_ORBIT_ALL = FLAG_ORBIT_POINT | FLAG_ORBIT_SPEED | FLAG_ORBIT_ALTITUDE
	};
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(Pattern,			pattern)
	AI_TASK_PARAMETER(E2::Vector,		point2)
	AI_TASK_PARAMETER(unsigned int,		flags)
	AI_TASK_PARAMETER(float,			speed)
	AI_TASK_PARAMETER(float,			altitude)
END_AI_TASK

//Landing at the given point. For helicopters only.
BEGIN_AI_TASK(Land,						Task::CATEGORY_NAV, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(bool,				durationFlag)
	AI_TASK_PARAMETER(wModelTime,		duration)
END_AI_TASK

//Refueling from the nearest tanker.
BEGIN_AI_TASK(Refueling,				Task::CATEGORY_NAV | Task::CATEGORY_SUPPLY, WORLDGENERAL_API)
END_AI_TASK

//Follow the friendly group
BEGIN_AI_TASK(Follow,					Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(unsigned int,		groupId)
	AI_TASK_PARAMETER(cVector,			pos)
	AI_TASK_PARAMETER(bool,				lastWptIndexFlag) //Last waypoint of the group to follow when the task is active
	AI_TASK_PARAMETER(int,				lastWptIndex)
END_AI_TASK


//external transportation cargo to the zone, for helicaopters
BEGIN_AI_TASK(CargoTransportation, Task::CATEGORY_SUPPLY, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string,	groupId)
	AI_TASK_PARAMETER(ObjectID,		zoneId)
END_AI_TASK


//Follow the friendly group and protect it from threats of the given types
BEGIN_AI_TASK(Escort,					Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(unsigned int,		groupId)
	AI_TASK_PARAMETER(cVector,			pos)
	AI_TASK_PARAMETER(bool,				lastWptIndexFlag) //Last waypoint of the group to follow when the task is active
	AI_TASK_PARAMETER(int,				lastWptIndex)
	AI_TASK_PARAMETER(float,			engagementDistMax)	
	AI_TASK_PARAMETER(TargetTypes,		targetTypes)
END_AI_TASK

//Background tasks

//Engage targets of the given type within the defined distance from the flight path. Attack parameters are provided
BEGIN_AI_BACKGROUND_TASK(EngageTargets, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(bool,				maxDistEnabled)
	AI_TASK_PARAMETER(float,			maxDistance)
	AI_TASK_PARAMETER(TargetTypes,		targetTypes)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
	AI_TASK_PARAMETER(int,				priority)
END_AI_TASK

//Engage targets of the given type in the given circle zone. Attack parameters are provided
BEGIN_AI_BACKGROUND_TASK(EngageTargetsInZone, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(float,			radius)
	AI_TASK_PARAMETER(TargetTypes,		targetTypes)
	AI_TASK_PARAMETER(AttackParam,		attackParam)
	AI_TASK_PARAMETER(int,				priority)
END_AI_TASK

//Engaging of the unit. Attack parameters are provided
BEGIN_AI_BACKGROUND_TASK(EngageUnit,	Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string,		unitId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)	
	AI_TASK_PARAMETER(int,				priority)
	AI_TASK_PARAMETER(bool,				visible)
END_AI_TASK

//Engaging of the group. Attack parameters are provided
BEGIN_AI_BACKGROUND_TASK(EngageGroup,	Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(unsigned int,		groupId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)	
	AI_TASK_PARAMETER(int,				priority)
	AI_TASK_PARAMETER(bool,				visible)
END_AI_TASK

//Make the group/unit a FAC and make it free to assign targets
BEGIN_AI_BACKGROUND_TASK(FAC,			Task::CATEGORY_COMMAND, WORLDGENERAL_API)
	AI_TASK_PARAMETER(float,			radius)
	AI_TASK_PARAMETER(int,				nCallsign)
	AI_TASK_PARAMETER(float,		    fFrequency)
	AI_TASK_PARAMETER(wModulation,		modulation)
END_AI_TASK

//Make the group/unit a FAC and let it assign the enemy group to the friendly CAS flight
BEGIN_AI_BACKGROUND_TASK(FAC_EngageGroup,Task::CATEGORY_COMMAND | Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(unsigned int,		groupId)
	AI_TASK_PARAMETER(AttackParam,		attackParam)	
	AI_TASK_PARAMETER(int,				priority)
	AI_TASK_PARAMETER(bool,				visible)
	AI_TASK_PARAMETER(DesignationMethod,designationMethod)
	AI_TASK_PARAMETER(bool,				useDatalink)
	AI_TASK_PARAMETER(int,				nCallsign)
	AI_TASK_PARAMETER(float,		    fFrequency)
	AI_TASK_PARAMETER(wModulation,		modulation)
END_AI_TASK

//Makes the unit/group an AWACS unit
BEGIN_AI_BACKGROUND_TASK(AWACS,			Task::CATEGORY_COMMAND, WORLDGENERAL_API)
END_AI_TASK

//Makes the unit/group a tanker
BEGIN_AI_BACKGROUND_TASK(Tanker,		Task::CATEGORY_SUPPLY, WORLDGENERAL_API)
END_AI_TASK

//Firing shells/rockets at the given point. For groups of artillery units only
BEGIN_AI_TASK(FireAtPoint,				Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(E2::Vector,		point)
	AI_TASK_PARAMETER(float,			radius)
	AI_TASK_PARAMETER(ed::string,		templateId)
	AI_TASK_PARAMETER(unsigned int,		weaponType)
	AI_TASK_PARAMETER(bool,				expendQtyEnabled)
	AI_TASK_PARAMETER(unsigned int,		expendQty)
END_AI_TASK

//Orders the ground group to hold
BEGIN_AI_TASK(Hold,						Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string,		templateId)
END_AI_TASK

//Orders the ground group to embark to transport
BEGIN_AI_TASK(EmbarkToTransport, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string,		templateId)
	AI_TASK_PARAMETER(cVector,		    embarkPoint)
	AI_TASK_PARAMETER(float,			embarkZoneRadius)
	AI_TASK_PARAMETER(ed::string,		selectedType)
	AI_TASK_PARAMETER(ObjectID,			selectedUnit)
END_AI_TASK

//Player-invisible task, order the ground group to whait for transport
BEGIN_AI_TASK(EmbarkToTransportWhaitForTransport, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string, templateId)
    AI_TASK_PARAMETER(AI::TaskPtr,      finalMission)
    AI_TASK_PARAMETER(cVector,		    embarkPoint)
	AI_TASK_PARAMETER(float,			embarkZoneRadius)
    AI_TASK_PARAMETER(cVector,		    disembarkPoint)
	AI_TASK_PARAMETER(float,			disembarkZoneRadius)
	AI_TASK_PARAMETER(ed::string,		selectedType)
	AI_TASK_PARAMETER(ObjectID,			selectedUnit)
    AI_TASK_PARAMETER(ObjectID,			mapSmokeMarkerId)
END_AI_TASK

//Player-invisible task, order the ground group to whait for transport
BEGIN_AI_TASK(EmbarkToTransportLoadToTransport, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string, templateId)
    AI_TASK_PARAMETER(AI::TaskPtr,      finalMission)
    AI_TASK_PARAMETER(cVector,		    embarkPoint)
	AI_TASK_PARAMETER(float,			embarkZoneRadius)
    AI_TASK_PARAMETER(cVector,		    disembarkPoint)
	AI_TASK_PARAMETER(float,			disembarkZoneRadius)
	AI_TASK_PARAMETER(ed::string,		selectedType)
	AI_TASK_PARAMETER(ObjectID,			selectedUnit)
    AI_TASK_PARAMETER(ObjectID,			transportID)
    AI_TASK_PARAMETER(cVector,          embarkingTransportPosition)
    AI_TASK_PARAMETER(ObjectID,			mapSmokeMarkerId)
	AI_TASK_PARAMETER(bool,				correctDisembarking)
	AI_TASK_PARAMETER(bool,				isGroupOnBoard)
END_AI_TASK

//Orders the ground group to disembark from transport
BEGIN_AI_TASK(DisembarkFromTransport, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(ed::string,		templateId)
	AI_TASK_PARAMETER(cVector,		    disembarkPoint)
	AI_TASK_PARAMETER(float,			disembarkZoneRadius)
    AI_TASK_PARAMETER(AI::TaskPtr,      finalMission)
    AI_TASK_PARAMETER(cVector,			transportPosition)
    AI_TASK_PARAMETER(ObjectID,			transportID)
END_AI_TASK

typedef ed::map<ObjectID,ed::set<ObjectID>> map_int_set_int;

//task for flying and embarking
BEGIN_AI_TASK(Embarking, Task::CATEGORY_ATTACK, WORLDGENERAL_API)
	AI_TASK_PARAMETER(cVector,		                point)
    //юниты которых нужно посадить
    AI_TASK_PARAMETER(ed::set<ObjectID>,            groupsId)
    AI_TASK_PARAMETER(map_int_set_int,              distribution)
	AI_TASK_PARAMETER(bool,				            durationFlag)
	AI_TASK_PARAMETER(wModelTime,		            duration)
    AI_TASK_PARAMETER(void,		(*sendCommand)(ObjectID passangerLeaderId, ObjectID transportId))
    //миссия продолжения маршрута после всех высадок
    AI_TASK_PARAMETER(AI::TaskPtr,                  finalMission)

END_AI_TASK



//Makes the ground group a EWR unit
BEGIN_AI_BACKGROUND_TASK(EWR,			Task::CATEGORY_COMMAND, WORLDGENERAL_API)
END_AI_TASK





class ITaskHelper
{
public:
	ITaskHelper() : _objId(-1), _wZad(-1), _wZadSave(-1), _pMaster(NULL) {;}
	ITaskHelper( ObjectID objId, int wZad, void * pObject ) : _objId(objId), _wZad(wZad), _wZadSave(wZad), _pMaster(pObject) {;}
	virtual ~ITaskHelper() {;}
	virtual void callFinish( TaskState *pTaskState ) = 0;
	virtual void callStart( TaskState *pTaskState ) = 0;
	virtual void callPause( TaskState *pTaskState ) = 0;
	virtual void callResume( TaskState *pTaskState ) = 0;

protected:
	void * _pMaster;
	ObjectID _objId;
	int _wZad;
	int _wZadSave;
};

BEGIN_AI_MULTI_TASK(MyStack, WORLDGENERAL_API)
	AI_TASK_PARAMETER(TaskState*,	prevTaskState)
	void push(const Task * pTask);
	TaskState * pTaskState;
	ITaskHelper * _pHelperCallback;
	ObjectID _idStack;
END_AI_TASK


AI_MULTI_TASK_STATE_BEGIN(MyStack,			Controller)
	AI_TASK_STATE_FINAL(MyStack, Controller)
	virtual bool isBackGround() const;
public:
	virtual void pushFrontRoute_(Task * pTask, ITaskHelper *pCallback = NULL, ObjectID nIdTask = -1);
	virtual void outerOnTaskStart_(TaskState * taskState);
	void pushBackRoute_(Task * pTask, ITaskHelper *pCallback = NULL, ObjectID nIdTask = -1);
	void pushFrontAttack_(Task * pTask, ITaskHelper *pCallback = NULL, ObjectID nIdTask = -1);
	virtual void pushBackAttack_(Task * pTask, ITaskHelper *pCallback = NULL, ObjectID nIdTask = -1);
	void popRoute_();
	void popAttack_();
	void next_();
	void addBackGroundTask_(BackGroundTask * pBackGroundTask);
	virtual void removeAllRoute_();
	virtual void removeTask_( ObjectID nIdTask );
	virtual void removeTask_( Task * pTask );
	virtual bool removeRouteTask_( Task * pTask );
	virtual bool removeAttackTask_( Task * pTask );
	virtual int isExistTask( Task * pTask );
protected:
	struct RunningTask
	{
		RunningTask(Task * pTaskIn, ITaskHelper *pCallback, ObjectID nIdTask) : pTask(pTaskIn), pTaskState(NULL), _pHelperCallback(pCallback), _nIdTask(nIdTask) {;}
		RunningTask(Task * pTaskIn) : pTask(pTaskIn), pTaskState(NULL), _pHelperCallback(NULL), _nIdTask(-1) {;}
		RunningTask(TaskState * pTaskStateIn) : pTask(pTaskStateIn->getTask()), pTaskState(pTaskStateIn), _pHelperCallback(NULL), _nIdTask(-1) {;}
		bool		run(Controller * pController);
		void		stop();
		void		save();
		void		clear();
		inline Category	getCategory() const { return pTaskState != NULL ? pTaskState->getCategory() : Task::CATEGORY_VOID; }
		inline bool		isBackGround() const { return pTaskState != NULL ? pTaskState->isBackGround() : pTask->isBackGround(); }
		Task*	pTask;
		TaskState * pTaskState;
		ITaskHelper * _pHelperCallback;
		ObjectID _nIdTask;
	};
	typedef ed::deque<RunningTask> RunningTasksStack;
	RunningTasksStack runningRouteTasksStack;
	RunningTasksStack runningAttackTasksStack;
	typedef ed::list<TaskState*> RunningBackGroundTasks;
	RunningBackGroundTasks runningBackGroundTasks;
AI_TASK_STATE_END

}

#endif __Common_Tasks__
