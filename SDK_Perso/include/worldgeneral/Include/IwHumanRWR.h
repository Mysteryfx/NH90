#pragma once 

// Radar Warning Receiver interface

#include "WorldGeneral.h"
#include "Registry\Registered.h"

// Radar Warning Receiver event
class RWR_event
{
public:
	RWR_event(ObjectID c, unsigned int t, unsigned int m, float d) : 
		client(c), RadarType_(t), RadarMode_(m), dist_(d) {}

	ObjectID client;
	unsigned int RadarType_;
	unsigned int RadarMode_;
	float dist_;
};

typedef ed::list<RWR_event> RWR_events_queue;

class WORLDGENERAL_API IwHumanRWR
{
public:
	virtual ~IwHumanRWR() = 0;
	virtual	void push_event(RWR_event) = 0;
	virtual	RWR_events_queue& get_RWR_events_queue() = 0;
};
