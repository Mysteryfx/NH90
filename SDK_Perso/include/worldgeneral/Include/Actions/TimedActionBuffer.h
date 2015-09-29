#ifndef __TIMED_ACTION_BUFFER_H__
#define __TIMED_ACTION_BUFFER_H__

#include <queue>

#include "ActionEntry.h"
#include "../../../WorldGeneral/Include/Tracks/track_event_consumer.h"

#include "wTime.h"

class TrackController;
class ActionDispatcher;

// Сюда складываются события ввода, имеющие метку времени. Они все в свое время пойдут в ActionDispatcher
// Из трека события тоже приходят сюда
// События с временем time обрабатываются ДО всех будильников с временем time
class WORLDGENERAL_API TimedActionBuffer : public ITrackEventsConsumer, public wTime::IStepCallback
{
public:
	TimedActionBuffer(TrackController* track, ActionDispatcher* dispatcher);
	~TimedActionBuffer();

    // Поставить в очередь команду из инпута
	void scheduleInputAction(const TimedActionEntry& entry);
    // Поставить в очередь стороннюю команду (не из инпута и не из трека)
    // Эти команды будут поставлены в очередь на момент времени GetModelTime() + 0.001
    void scheduleAction  (const ActionEntry& entry, ActionOrigin src);
    // Эти команды считаются пришедшими из мира
    void scheduleAnalogAction (Action action, double value);
    void scheduleDigitalAction(Action action);

    // Скормить диспетчеру события до time (включительно)
    void feedEventsUpTo(double time);	

	// ITrackEventsConsumer
	virtual bool ITrackEventsConsumer::filterEvent(int id);
	virtual void ITrackEventsConsumer::consumeEvent(double time, const Mail::Message& msg);

	// IPrestepCallback
	virtual void wTime::IStepCallback::onStep(wModelTime time);

private:
	// Три очереди надо, потому что события от инпута, трека и мира могут перемешиваться
	std::queue<TimedActionEntry> inputActions_;	
	std::queue<TimedActionEntry> trackActions_;
    std::queue<std::pair<TimedActionEntry, ActionOrigin> > otherActions_;

	ActionDispatcher* dispatcher_;
};

#endif