#pragma once 

#include "Inter.h"

typedef unsigned int ObjectID;

#define S_EVENT_BEGIN enum sEventType {
#define S_EVENT(event) event,
#define S_EVENT_END };

#include "SEvent.h"

#undef S_EVENT_END
#undef S_EVENT
#undef S_EVENT_BEGIN

extern INTER_API const char * sEventTypeStr[S_EVENT_MAX];

class INTER_API IScoreList
{
public:
	virtual ~IScoreList() = 0 {};
	virtual void start() = 0;
	virtual void stop() = 0; // переводит события в debriefEvents
	virtual void serializeDebriefing(const char *debrief_file) const = 0;

	virtual void createBirthEvent(ObjectID newborn, ObjectID place, int subPlace) = 0;
	virtual void createDamageEvent( ObjectID shooter_, ObjectID weapon_, ObjectID target_ ) = 0;
	virtual void createShotEvent( ObjectID shooter_, ObjectID weapon_, const ed::string& s_weapon_, const ed::string& s_weapon_type_ ) = 0;
	virtual bool detailedShotsLog() = 0;

	virtual void startShooting  ( ObjectID shooter_,ObjectID target_, const ed::string& s_weapon_) = 0;
	virtual void endShooting  ( ObjectID shooter_,const ed::string& s_weapon_) = 0;

	virtual void createCrashEvent(ObjectID object_) = 0;
	virtual void createEjectEvent(ObjectID plane_) = 0;
	virtual void createRefuelingEvent(ObjectID plane_) = 0;
	virtual void createTakeOffEvent(ObjectID plane_, ObjectID airfield_) = 0;
	virtual void createLandEvent(ObjectID plane_, ObjectID airfield_) = 0;
	virtual void createPilotDeadEvent(ObjectID plane_) = 0;
	virtual void createDestroyEvent(ObjectID id) = 0;
	virtual void createBaseCaptureEvent(ObjectID  id, ObjectID base_) = 0;
    virtual void createMissionStartEvent() = 0;
    virtual void createMissionEndEvent(const ed::string& winner, const ed::string &msg) = 0;
    virtual void createTookControlEvent() = 0;
    virtual void createRefuelingStopEvent(ObjectID plane_) = 0;
	virtual void createHumanFailureEvent( ObjectID plane_,const ed::string& s_failureID, const ed::string& s_failureDisplayName) = 0;
	virtual void createHumanEngineStartupEvent(ObjectID plane_, ObjectID base_) = 0;
	virtual void createHumanEngineShutDownEvent(ObjectID plane_, ObjectID base_) = 0;

	virtual void onPlayerEnterUnit(const ed::string & unit_mission_id) = 0;
	virtual void onPlayerLeaveUnit(const ed::string & unit_mission_id) = 0;
   //////////////////////////////////////////////////////////////////////////
    // network stats wrapper functions
    enum stats_categories_t
    {
        STAT_INVALID = 0,
        STAT_PLANE,
        STAT_CAR,
        STAT_SHIP,
        STAT_STATIC,
        STAT_CRASH,
		STAT_EJECT,
        STAT_OVERALL_SCORE,
        STAT_LAST,
    };
    typedef stats_categories_t ScoreID_t;
    typedef int PlayerID_t;
    typedef int Score_t; // может быть отрицательным

    // Установить callback.
    // Должен вызываться при изменении статистики игрока.
	class ScoreCB {
	public:
		virtual void scEject(PlayerID_t) = 0;
		virtual void scCrash(PlayerID_t) = 0;
		virtual void scKill(PlayerID_t who, const ed::string& weapon, const ed::string& victim) = 0;
		virtual void scTakeoff(PlayerID_t, const ed::string& airdrome) = 0;
		virtual void scLanding(PlayerID_t, const ed::string& airdrome) = 0;
		virtual void scScore(PlayerID_t, ScoreID_t, Score_t new_value) = 0;
		virtual void scMissionEnd(const ed::string& winner, const ed::string &msg) = 0;
		virtual void scDamage(ObjectID shooter, ObjectID weapon, ObjectID victim) = 0;
		virtual void scKillPlayer(PlayerID_t whom, const ed::string& weapon, const ed::string& killa) = 0;
	};

    virtual void set_callback(ScoreCB* cb) = 0;

    // Добавить игрока. Все очки - по нулям.
	// @return false - player already exists
    virtual bool add_player(PlayerID_t) = 0;

    // Удалить игрока (и его статистику).
    virtual void del_player(PlayerID_t) = 0;

    // Установить "юнит" игрока, т.е. объект, чьи заслуги будут записываться 
    // игроку. Пустой UnitID - значит нет юнита.
    // Может вызываться произвольное число раз.
    virtual void set_unit(PlayerID_t, const ed::string& UnitID) = 0;

    // Получить статистику по игроку:
    virtual Score_t get_score(PlayerID_t, ScoreID_t) = 0;

    virtual void write_debriefing(const char *str) = 0;
};

extern INTER_API IScoreList *globalScoreList;
