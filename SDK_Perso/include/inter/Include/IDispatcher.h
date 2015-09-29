#ifndef __IDispatcher_h__
#define __IDispatcher_h__

#include "Inter.h"
#include "wTime.h"
#include "dPublic.h"
#include "Registry/Registered.h"

struct lua_State;

namespace Mail { class Message; }
namespace Mission { class MissionPackage; }

class TrackController;

class ActionSource;
class ActionDispatcher;
class TimedActionBuffer;


typedef unsigned UnitProperty;

typedef unsigned start_command_; // loadMission()

// Интерфейс диспетчера
class INTER_API IDispatcher 
{
public :
    virtual ~IDispatcher() = 0 {};

	virtual void    SetNetGui(bool on) = 0;
	virtual void    SetStart( void ) = 0;
	virtual void    SetRestart( void ) = 0;
	virtual void    SetStop( void ) = 0;
	virtual void    SetFinish( void ) = 0;  
	virtual void    ReloadDatabase( void ) = 0;
    virtual void    Construct() = 0;
	virtual int     Init(int stage)  = 0;       // Инициализация
    virtual void    Start() = 0;      // Переход из режима работы в редакторе миссий к имитации - Now it is wrong simetimes	
	virtual void	PostStart( lua_State * ) = 0;   // Окончательный переход к имитации (хорошо бы переименовать Start -> Prestart, а PostStart -> Start)
	virtual bool    Next()  = 0;       // Вычисление следующего кадра
    virtual void    NextFrame() = 0;  // Работа в режиме имитации
    virtual void    Stop() = 0;       // Переход от режима имитации к работе с редактором миссий - Now it is wrong simetimes
	virtual void    Finish() = 0;     // Деинициализация
    virtual const	dMode_e& GetMode() const = 0;
    virtual void    SetMode(dMode_e Mode) = 0;
	virtual void	setProfilingStart(const ed::string &path) = 0;
    virtual void      SetOneFrameStep(const wModelTime& stepValue) = 0;
	virtual const unsigned int getProgramVersion() = 0;
	virtual unsigned int getCurrentVersion() = 0;
	virtual void setCurrentVersion(unsigned int) = 0;
    virtual bool loadMission(const ed::string& fileName, const bool start = true) = 0;
	virtual unsigned int getMissionVersion() = 0; 

    // Поскольку снаружи проекта AviController не доступен, вводятся эти 
    // вспомогательные функции
    virtual bool isRecordingAvi() = 0;

	virtual void post_message(const Mail::Message& msg) = 0;
    virtual lua_State* mission() = 0; // stand-alone state, contains mission, units DB, run-time DB

    virtual lua_State* getLuaState(const char *name) = 0; // get lua state by name

	virtual TrackController* getTrackController() = 0;
	virtual Mission::MissionPackage* getMission() = 0;
    virtual void InterruptTrack() = 0; // interrupt track, proceed in mission mode (take control)

	virtual ActionSource* getActionSource() = 0;
	virtual TimedActionBuffer* getActionBuffer() = 0;
	virtual ActionDispatcher* getActionDispatcher() = 0;

    virtual bool register_ID(const ed::string& name_, int ID_) = 0;
    virtual ed::string get_unit_property(const ed::string& misId,const UnitProperty  & prop) = 0;
    virtual ed::string get_unit_property(const ObjectID id_,const UnitProperty  & prop) = 0;
    virtual ed::string get_unit_name(const unsigned int id_) = 0; 
    virtual ObjectID get_unit_ID(const ed::string& name) = 0;

	virtual ObjectID get_shooter_id(ObjectID weapon) = 0;
    virtual void WriteTrackFile() = 0;
    virtual bool getNeedRestartApplication() const = 0;

    virtual bool getPause() = 0;
    virtual void setPause(bool do_pause) = 0;
};

// Глoбальный диспетчер программы
extern INTER_API IDispatcher *globalDispatcher;


#endif /* __IDispatcher_h__ */
