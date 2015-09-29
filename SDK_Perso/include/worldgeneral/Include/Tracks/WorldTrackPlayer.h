#pragma once
#include "WorldGeneral.h"
#include "Tracks/track_event_consumer.h"

class WORLDGENERAL_API WorldTrackPlayer : public ITrackEventsConsumer 
{
public:
	WorldTrackPlayer();
	~WorldTrackPlayer();

	bool filterEvent(int id);
	void consumeEvent(double time, const Mail::Message& msg);
};