#ifndef __TRACK_CONTROLLER_H__
#define __TRACK_CONTROLLER_H__

#include "WorldGeneral.h"

#include "track_event_consumer.h"
#include "../Mission/track.h"
#include "../Mission/mission_storage.h"

#include "wTime.h"

namespace Mission { class MissionPackage; }

// Управляет воспроизведением и сохранением треков
class WORLDGENERAL_API TrackController : public wTime::IStepCallback
{
public:
	TrackController(bool reading, bool writing, Mission::MissionPackage* mission);
	~TrackController();

	// State query
	bool isActive();		// Есть ли трек (на запись или проигрывание)
	bool isReading();		// Проигрываем?
	bool isWriting();		// Пишем?

	void stopReading();		// Прекратить чтение трека

	// Запись
	void pushEvent(Mission::TrackEvent& ev);

	// Чтение
	void addConsumer(ITrackEventsConsumer* consumer);
	void removeConsumer(ITrackEventsConsumer* consumer);
	// Скормить потребителям события до времени time включительно
	void feedEvents(double max_time);
	// Это надо для сети...
	// Вообще-то кривовато (см. TrackPlayer)
	bool hasEvents(double time);
	Mission::TrackEvent popEvent();
	Mission::TrackEvent peekEvent();

    // Дополнительные данные (функции вызывают соответствующие из missionPackage)
/*    void addChunk(const ed::string& name, const Mission::MissionData& data);
    Mission::MissionData getChunk(const ed::string& name);
    void loadLuaChunk(const ed::string& name, lua_State* state);*/


	// Всякое разное
	double getLastEventTime() const;

	// wTime::IStepCallback
	virtual void wTime::IStepCallback::onStep(wModelTime time);

protected:
    class TrackActivity*                activity_; 

	ed::vector<ITrackEventsConsumer*>	consumers_;
	Mission::MissionPackage*			mission_;
	Mission::Track						*readTrack_, *writeTrack_;

	Mission::TrackEvent					eventCache_;
	bool								hasEvent_;
};

#endif