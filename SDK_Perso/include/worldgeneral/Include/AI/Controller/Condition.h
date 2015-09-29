#ifndef __Condition__
#define __Condition__

//Condition for task. Used in ControlledTask decorator

#include "Randomizer\RandomObject.h"
#include "wTime.h"

namespace Lua
{
	class Config;
}

namespace AI
{

class Condition
{
public:
	Condition();
	Condition(const Condition & condition);
	~Condition();
	Condition & operator = (const Condition & condition);	
	void load(Lua::Config & config);
	inline bool isEnabled() const { return timeEnabled || probabilityEnabled || userFlagEnabled || scriptEnabled; }
	bool check() const;
private:
	bool			timeEnabled;
	wModelTime		time; //Start time
	bool			probabilityEnabled;
	float			probability; //Probability of true
	bool			userFlagEnabled;
	ed::string		userFlag; //User flag id to check
	bool			userFlagValue; //Required user flag value
	bool			scriptEnabled;
	ed::string		script; //lua-predicate
	static RandomObject randomEvenly;
};

typedef Condition StartCondition;

class StopCondition : public Condition
{
public:
	StopCondition();
	void load(Lua::Config & config);
	inline bool isEnabled() const { return Condition::isEnabled() || durationEnabled || lastWptEnabled; }
	bool check(wModelTime taskStartTime, int fromWaypoint) const;
private:
	bool			durationEnabled;
	wModelTime		duration; //The limit of the task duration
	bool			lastWptEnabled;
	unsigned int	lastWaypoint; //The last waypoint then the background task is active
};

}

#endif __Condition__
