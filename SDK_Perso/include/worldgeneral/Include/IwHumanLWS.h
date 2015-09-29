#pragma once 

// Laser Warning System interface

#include "WorldGeneral.h"
#include "Registry\Registered.h"

// Laser Warning System event
class LWS_event
{
public:
	LWS_event(ObjectID c, unsigned int l) : client(c), LaserType_(l) {}

	ObjectID client;
	unsigned int LaserType_;
};

typedef ed::list<LWS_event> LWS_events_queue;

class WORLDGENERAL_API IwHumanLWS
{
public:
	virtual ~IwHumanLWS() = 0;
	virtual	void push_event(LWS_event) = 0;
	virtual	LWS_events_queue& get_LWS_events_queue() = 0;
};
