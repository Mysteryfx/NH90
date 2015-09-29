#ifndef __TRACK_H__
#define __TRACK_H__

#include <ed/string.h>
#include <ed/vector.h>

#include "WorldGeneral.h"

#include "Mail/Message.h"
#include "Misc/TempFilesManager.h"

namespace Mission
{

class MissionReader;
class MissionWriter;

struct TrackEvent
{
	TrackEvent(double time, const Mail::Message& event_) : time(time), event(event_) {}
	TrackEvent(double time, Mail::ID_t id) : time(time), event(id, 0) {}

	double time;
	Mail::Message event;
};

class ITrackEventFilter	// Используется для фильтрации сообщений при записи трека (в разные потоки чтоб писалось)
{
public:
	enum BoxStorage
	{
		STORE_TEXT,
		STORE_BINARY
	};
public:
	virtual ed::vector<std::pair<ed::string, BoxStorage> > getBoxes() const = 0;

	virtual int				getBoxIndex(int event_id) const = 0;
};

// Кусок трека (то, куда фильтрует ITrackEventFilter)
class TrackFiber;
struct TrackEvent_;

// В трек нельзя одновременно читать и писать
class WORLDGENERAL_API Track
{
public:
	static bool isTrackChunk(const ed::string& name);

public:
	Track(const ITrackEventFilter* filter);		// Трек для записи
	Track(MissionReader* reader);			// Трек для чтения
	~Track();

	void save(MissionWriter* writer);	

	bool reading() const { return filter_ == 0; }

	bool empty() const;

	bool hasEvents(double before);		// Есть ли события с time <= before. NB: внутри время сравнивается без EPS
	TrackEvent popEvent();				// Вытащить верхнее событие
	double getLastEventTime() const;	// Время последнего события

	void addEvent(const TrackEvent& event);

protected:
	struct FiberEntry_
	{
		TrackFiber* fiber;
		TrackEvent_* ev;
	};

	const ITrackEventFilter* filter_;
	ed::vector<FiberEntry_> fibers_;
	CoreUtils::TempFilesManager tempFiles_;

	int cnt_;
};

}

#endif
