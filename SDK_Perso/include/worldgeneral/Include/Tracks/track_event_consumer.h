#ifndef __TRACK_EVENT_CONSUMER_H__
#define __TRACK_EVENT_CONSUMER_H__

namespace Mail { class Message; }

class ITrackEventsConsumer
{
public:
	// фильтрация сообщений по id
	// true - принимает
	virtual bool filterEvent(int id) = 0;

	virtual void consumeEvent(double time, const Mail::Message& msg) = 0;
};


#endif