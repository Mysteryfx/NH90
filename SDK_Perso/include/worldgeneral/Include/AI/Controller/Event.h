#ifndef __Events__
#define __Events__

//Events to be handled by the Controller and the Task State(s) inside the Controller

namespace AI
{

enum Event
{
	EVENT_MESSAGE,
	EVENT_SHOOT,
	EVENT_DAMAGE,
	EVENT_TANKER,
	EVENT_WAYPOINT_CHANGED,
	EVENT_GROUP_UNIT_DEATH,
	EVENT_TAKEOFF,
	EVENT_MAXIMUM,
};

}

#endif