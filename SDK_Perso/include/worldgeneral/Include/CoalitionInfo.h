#pragma  once

#include <assert.h>

enum Role 
{
	ROLE_INSTRUCTOR = 0,
	ROLE_PILOT,
	ROLE_FORWARD_OBSERVER,
	ROLE_ARTILLERY_COMMANDER,
	ROLE_OBSERVER,
	ROLE_SPECTRATOR,
	ROLE_COUNT
};

enum wcCoalitionName
{ 
	COALITION_VOID = -1,
	COALITION_NEUTRAL, 
	COALITION_RED, 
	COALITION_BLUE, 
	COALITION_MAX 
};

inline bool            IS_HOSTILE_FOR_ME(int myCoalition,int target)
{
    switch(myCoalition)
    {
		case COALITION_RED:
			return target == COALITION_BLUE;
		case COALITION_BLUE:
			return target == COALITION_RED;
		default:
			return false;
    }
}

inline int GET_ENEMY_COALITION(int myCoalition)
{
	switch(myCoalition)
	{
		case COALITION_NEUTRAL:
			return COALITION_NEUTRAL;
		case COALITION_RED:
			return COALITION_BLUE;
		case COALITION_BLUE:
			return COALITION_RED;
		default:
			assert(false);
			return COALITION_NEUTRAL;
	}
}

inline int IS_WARSIDE(int myCoalition)
{
	return myCoalition == COALITION_RED || myCoalition == COALITION_BLUE;
}