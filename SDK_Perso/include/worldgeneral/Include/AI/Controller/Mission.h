#ifndef __Mission__
#define __Mission__

//Mission task means following route and performing list of actions which are corresponded to the waypoints of the route

#include <ed/vector.h>

#include "AI/Controller/Task.h"
#include "AI/Controller/TaskState.h"
#include "AI/Controller/Route.h"

namespace Lua
{
	class Config;
}

namespace AI
{

enum CommandEnum;

BEGIN_AI_MULTI_TASK(Mission, WORLDGENERAL_API)
	Mission(const Mission & mission);
	Mission & operator = (const Mission & mission);
	bool			airborne; //Is the route for airborne unit/group
	Route			route;
	int				goToIndex; //Index of the first waypoint
END_AI_TASK

AI_MULTI_TASK_STATE_BEGIN(Mission, Controller, WORLDGENERAL_API)
	AI_TASK_STATE_BASE(Mission, Controller)
public:
	void switchWaypoint(unsigned int fromWaypointIndex, unsigned int goToWaypointIndex);
	virtual void stopRoute();
	virtual void resumeRoute();
	inline int getFromIndex() const { return currentFromWaypointIndex; }
protected:
	enum
	{
		NO_WAYPOINT = -1,
		END_OF_ROUTE = -2
	};
	virtual bool isBackGround() const;
	virtual bool checkFinish_() const;
	virtual void onChangeWaypoint_(int fromWaypoint, int goToWaypoint);
	virtual void setRoute_();
	virtual void switchWaypoint_(int fromWaypoint, int goToWaypoint, bool startTask);
	virtual void enableRouteFollowing_(bool on);
	virtual void resetRoute_();
	void startTask_(int index);
	void finishTask_(int index);
	void onFinishTask_(int index);
	void onMainTaskStart_();
	void onMainTaskFinish_();
	int							currentFromWaypointIndex;
	int							currentGoToWaypointIndex;
	typedef ed::vector<bool>	TaskBackgroundFlags;
	TaskBackgroundFlags			taskBackgroundFlags;
	int							mainTaskQty;
AI_TASK_STATE_END

}

#endif __Mission__
