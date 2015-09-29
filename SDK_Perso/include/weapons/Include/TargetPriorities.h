#ifndef _target_priorities_
#define _target_priorities_

#include "wMovingObject.h"

#define INVALID_TARGET_PRIORITY -100000000.0f
#define TARGET_SWITCH_PRIORITY_THRESHOLD 0.1f // The difference in priority that new target needs to exceed over current one to switch WS on itself

struct UnitTargetPair
{
	woPointer ws_manager;
	unsigned int targetID;

    UnitTargetPair(): ws_manager(NULL), targetID(0) {};

    UnitTargetPair(woPointer& ws_manager_, unsigned int targetID_)
	{
		ws_manager = ws_manager_;
		targetID = targetID_;
	}

};

//typedef std::pair<float, UnitTargetPair> UnitTargetPriorityPair;
typedef std::multimap<float, UnitTargetPair, std::greater<float>> UnitTargetPriorityMap;
typedef UnitTargetPriorityMap::iterator UnitTargetPriorityMap_iter;
typedef UnitTargetPriorityMap::const_reverse_iterator UnitTargetPriorityMap_rev_iter;

struct WSTargetPair
{
	unsigned int wsID;
	unsigned int targetID;

	WSTargetPair(unsigned int wsID_, unsigned int targetID_)
	{
		wsID = wsID_;
		targetID = targetID_;
	}
};

//typedef std::pair<float, WSTargetPair> WSTargetPriorityPair;
typedef std::multimap<float, WSTargetPair, std::greater<float>> WSTargetPriorityMap;

typedef WSTargetPriorityMap::iterator WSTargetPriorityMap_iter;

class TargetsPriorities;

typedef std::multimap<float, woPointer, std::greater<float>> priority_unit_t;
class TargetPriorities
{
    friend TargetsPriorities;
public:
    TargetPriorities(ObjectID trgID);

    inline bool             isEmpty() const { return priority_unit.empty(); };
    inline float            getTopRawPriority() const { return priority_unit.begin()->first; };
    inline float            getCurrentPriority() const { return currIt->first - m_handicap; };
    inline woPointer        getCurrentUnit() const { return currIt->second; };
    inline float            getCurrentRawPriority() const { return currIt->first; };
    bool                    finished() const { return currIt == priority_unit.end(); };
    ObjectID                getTargetID() const { return m_targetID; };

protected:

    void                    take(UnitTargetPair& unit_target);
    void                    updatePriority(float priority, const woPointer unit);
    float                   changeHandicap(float addHandicap);
    float                   resetHandicap();
    void                    reset();
    void                    removeMember(const woPointer unit_);
    void                    removeTargetPriority(const woPointer unit_);

    priority_unit_t priority_unit;
    priority_unit_t::iterator currIt;

    ObjectID m_targetID;
    double m_handicap;
};

typedef ed::list<TargetPriorities> target_priorities_t;
class TargetsPriorities
{
public:
    TargetsPriorities()     {};
    void                    clear();
    void                    updatePriority(const ObjectID trgID,  float priority, const woPointer unit);
    void                    reset();
    void                    changeHandicap(ObjectID trgID, float handValue);
    bool                    take(UnitTargetPair& unit_target);
    void                    removeMember(const woPointer unit_);
    void                    removeTargetPriority(const ObjectID ID_, const woPointer unit_);
    void                    removeTarget(const ObjectID ID_);
protected:
    target_priorities_t     target_priorities;
    
};

#endif
