#ifndef __wControl_h__
#define __wControl_h__

//Controller of an unit or of an entire group of units

//Subclasses identifiers
enum wControlClass
{
	wcClass_Control		= 1,
	wcClass_Country		= 2,
	wcClass_AirGroup	= 3,
	wcClass_Wing		= 4,
	wcClass_Pair		= 5,
	wcClass_Plane		= 6,
	wcClass_HumanPlane	= 7,
	wcClass_HumanHel	= 8,
	wcClass_Turret		= 11,
	wcClass_HelGroup	= 16,
	wcClass_Squadron    = 17,
	wcClass_Column	    = 18,
	wcClass_Helicopter	= 19,
	wcClass_HelWing		= 20,
	wcClass_AirNetGroup	= 21,
	wcClass_AirNetHelGroup = 22,
	wcClass_AirNetColumn	= 23,
	wcClass_AirNetSquadron	= 24,
	wcClass_Static		= 25
};

#include "WorldGeneral.h"
#include "Common.h"
#include "wTime.h"
#include "wMovingObject.h"

#include "AI/Controller/Controller.h"
#include "AI/Detection/wTargetDetectionStatus.h"
#include "wListener.h"

class wControl;
class wiPath;
class wTargetDetectionStatus;

class WORLDGENERAL_API wWakeupClient 
{
public:
	wWakeupClient():timer(0){};
	virtual ~wWakeupClient(){if (timer) delete timer;};
	void			wake_me_up(wModelTime time_); // разбудить группу по таймеру
	void			wake_me_up_now(); // разбудить группу по триггеру
	virtual wModelTime		Control(){return 0;}; // функция, которую вызовет будильник. возвращает время, через которое проснуться в след раз
private:
	wtPointer		timer;	    // деятельность Control() в модельном времени	
};

class WORLDGENERAL_API wWakeupTime : public wTime
{
public:
    wWakeupTime::wWakeupTime(wWakeupClient* p, wModelTime dt);
    virtual ~wWakeupTime();

    virtual void NextEvent();
    wWakeupClient* boss;
};

class wDetector;

namespace AI
{
class Task;
}

class WORLDGENERAL_API wControl :	public AI::Controller,
									public wListener, 
									public Suicide,
									public wWakeupClient
{
public:
	enum Category
	{
		CATEGORY_VOID = -1,
		CATEGORY_AIRPLANE,
		CATEGORY_HELICOPTER,
		CATEGORY_VEHICLE,
		CATEGORY_SHIP
	};
	typedef wControlClass		wClass;
	typedef woPointer			Unit;
	typedef struct Target
	{
		Unit ptr;
		wTargetDetectionStatus detectionStatus;
	};
	typedef ed::vector<Unit>	Units;
	typedef ed::vector<Target>	Targets;
	typedef ed::list<wControl*> wControls;
private:
	typedef ed::map<ObjectID, wControl*> StorageId;
	static StorageId storageId;
	typedef ed::map<ed::string, wControl*> StorageName;
    static StorageName storageName;
public:
	//wControl storage: by id and by name
    static wControl*    getByMissionId(ObjectID id);
    static ObjectID     getMissionIdByControl(const wControl * pControl);
	static wControl*    getByName(const ed::string & name);
    static ObjectID     getNextMissId();
    static void         acuireMissionId(ObjectID id, wControl * pControl);
    static void         unregisterMissionId(wControl * pControl);

	wControl();
	virtual ~wControl();
	virtual void			serialize(Serializer &);
	virtual void            destroy(bool immediately = false);

	//AI::Controller
	GET_DESC_METHOD(wControl, AI::Controller)
	void					finishTask();

	//Group triggered actions from the Mission Editor
	void					loadTriggeredTasks(Lua::Config & config);
	virtual bool			triggerTask(unsigned int taskNumber, bool push);

	//Returns wDetector
	virtual wDetector *		getDetector();

	//Update
	virtual wModelTime		Control(); // функция, которую вызовет будильник. возвращает время, через которое проснуться в след раз
	
    //World event handlers
	virtual void			onShoot(wcCoalitionName coalition, ObjectID shooterID, const wsType & weaponType, void * weaponData, ObjectID weaponID, ObjectID targetID, const cPoint & targetPoint, wModelTime impactTime);
	virtual void			onDamage(wcCoalitionName coalition, ObjectID shooterID, ObjectID weaponID, ObjectID targetID, float deltaLife);
	virtual bool			detectMissile(woPointer pMissile, bool missileIsRadiating) { return false; }
	virtual void			forceDetectMissile(woPointer pMissile) { ; }

	//Mission id and name
	void					setName(const ed::string & nameIn);
	inline const ed::string & getName() const { return name; }

	//Status
	virtual bool			isDead() const;
	virtual wcCoalitionName	Coalition() const;
	Category				getCategory() const;
	virtual wClass			Class() const { return wcClass_Control; }
	bool					isGroup() const;

	//Returns parent commander
	virtual wControl*		GetCommander() const;

	//Returns leader and units
	virtual Unit			getLeader() const;
	virtual Unit			getUnit(int number) const;
	virtual int				getSize() const;
    int                     getInitialUnitsSize() { return nInitialUnitsSize; }
    void                    setInitialUnitsSize(int nSize) { nInitialUnitsSize = nSize; }

	//Returns children commanders
	const wControls &		getChilds() const { return children; }

	//Returns detected targets
	virtual void			getDetectedTargets(Targets & targets) const;

	//Returns information about the contact with the target
	virtual bool			isTargetDetected(ObjectID targetID, wTargetDetectionStatus & targetDetectionStatus) const;

	//Forces the commander to know about target
	virtual void			knowTarget(const woPointer & pTarget, unsigned char flags = wcDetFlagTypeIsRecognized | wcDetFlagDistanceIsKnown);
	
	wModelTime				StartTime;	// Время активизации группы от старта миссии

    virtual void            validatePointerOnObjectID(ObjectID ID_, woPointer woPnt); // related to player unit switch
   
protected:
	wControls				children; //chilren commanders
	ed::vector<AI::Task*>	triggeredTasks;
	ed::string				name;
    int                     nInitialUnitsSize;
};

typedef cPointerTemplate<wControl>	wcPointer;

#endif
