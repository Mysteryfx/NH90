#ifndef __wcColumn_h__
#define __wcColumn_h__

#include <ed/vector.h>
#include <ed/map.h>

#include "Transport.h"
#include "wControl.h"
#include "wListener.h"
#include "wActivityManager.h"
#include "IwcCountry.h"
#include "wGroupTargetManager.h"
#include "wiPath.h"
#include "LandObjects/lBridge.h"

#include "Registry/Registered.h"
#include "IwNetObject.h"
#include "wcColumnMail.h"
#include "wcColumnData.h"
#include "SurfaceGroup\wcRoute.h"

#include "wCarEventualTargets.h"

class wcRoute;
class wWorld;
class wcColumn;
class LandGroup;
class woCar;
class woMovingVehicle;
class Vector3;
class lRoadDestroyedObject;
// panov {
class wWeaponSystem;
class cLauncher;
// panov }

struct TaxiPoint;
enum CarFormationType;
enum LandGroupStateType;
enum GTMAlarmType;
enum ROEType;

namespace AI
{
	class Mission;
}

const int NumObjMax = 1000;

#define IID_IColumn Common::identify<wcColumn>() 

class TRANSPORT_API wcColumn : public Registered, public IwNetObject, 
	public wGroupTargetManager, public BridgeListener//, public wCarEventualTargets
{
public:

	wcColumn(); 
	virtual ~wcColumn();

	GET_DESC_METHOD(wcColumn, wControl)

    static wcColumn*    create(lua_State* meGroup_, IwcCountry* country_, bool master_, unsigned char countryID_);

	virtual void		setOnOff(bool onOff);
    void                setOption(unsigned int name, const Option & value);

    void                setCountry(lua_State* meGroup_, IwcCountry *pC, unsigned char countryID_, bool lateActivation = false);

	void				addMember(woCar* object);
	void				removeMember(woCar* object);
	void				setMemberReconMode(woCar* object, bool on);
    void                updateNumberOfEnabledAIs(bool on_); // используется в woCar для принудительного включения/отключения активностей при включении/отключении отдельных ИИ машинок

	const wcRoute*		groupRoute() const {return route;}
	const NM_ROAD::Road	*groupPath() const;
	const woMovingVehicle* vehicle(int groupIndex) const;
	double				leaderPathParam() const;
	double				firstCarPathParam(bool useCache = true) const;
	double				leaderSpeed() const;
	bool				isColumn() const;
	bool				isOnRoad(float offset) const;
	bool				isEndOfPath() const;
	bool				isAllStaying() const;

	void				setRoute(const wcRoute::RouteType & waypointsIn);
	void				setRouteSpeed(double speed);
	double				routeSpeed() const;

	CarFormationType	currentFormation() const;
	void				setFormation(CarFormationType form);

	ROEType				currentROE() const;
	void				setROE(ROEType);

	GTMAlarmType		currentAlarmState() const;
	void				setAlarmState(GTMAlarmType);

    virtual int         GetLastWaypointId() const; // get the number of last waypoint we passed
	inline int			getRoutesize() const { return waypoints.size(); }

	wControl::Unit		getLeader() const;
    wControl::Unit		getUnit(int number) const;
    virtual int			getSize() const { return carList.size(); }

#ifdef DEBUG_TEXT
	virtual const char* debug_text();
#endif
	//wControl interface begin	
    virtual wModelTime		Control(); // функция, которую вызовет будильник. возвращает время, через которое проснуться в след раз
	wcCoalitionName			Coalition() const { return my_coalition; }
	wClass					Class() const { return wcClass_Column; }
	virtual bool			isDead() const;
	virtual wDetector *		getDetector();
	//
	double					getMaxFireAltitude(ObjectID targetID) const;
	//
	const wiPath*			getPath() const;
	float					getPathParam() const;

	//wControl interface end

	bool					checkedDisperse();
	void					reform();

	void fallInTemplate(const ed::string& templId);

	void increaseColumnLength();
	void resetColumnLength();

	virtual void stopMovement();
	virtual void continueMovement();

	LandGroupStateType state() const;

	void wait(double delay);//установится на время delay
	void setPathTo(const PathOptions& opt);
	//Выставить прямой путь (как по двум точкам в редакторе + объезд, если есть препятсвия)
	void setStraightPathTo(const PathOptions& opt);
	void clearPath();
	void setPathHandler(bool routeExist, wcRoute *newRoute, int requestLocalId);
	void setByPassRouteHandler(wcRoute *newRoute, double goToPointParam, ObjectID bridgeId, double bridgeParam);

    void netHideGroup(bool b);

	void freezePath();

    // артнаводчик
	void addFOTarget(ObjectID targetId);
	void removeFOTarget(ObjectID targetId);
	void disableFOTargets();
	bool isFOTargetsAssignable() const {return isFOPathAssignable() ? foTargetsAssignable : false;}
	bool isFOPathAssignable() const {return foPathAssignable;}

	void	removeFromAIColumn(woCar*);
	void	addToAIColumn(woCar*);
    void    rotateLeaderPosition();

    virtual void    destroy(bool immediately = false);

	//Сеть
	int netGetHost() const;
	bool netGetCreate(Mail::Message&) const;
	bool netSendUpdate(int dest, int prio, int& cookie);
	void netDiscard();

	void OnRoadObjectDestroyed(const lRoadDestroyedObject* bridge);

	// panov {
	virtual int getCarSize() { return carList.size(); }
	bool isLeader( woCar *pCar ) {
		if( carList.size() && carList[0] == pCar )
			return true;
		return false;
	}

	/*
	virtual woCar *getCar( int nIdx ) { 
		if( carList.size() > nIdx )
			return carList[nIdx];
		return NULL;
	}
	
	virtual woCar *getTargetCar( int nTargetId, cLauncher **_pLauncher, bool bWeaponManager = false );
	*/
	bool			getFireTimeTargetCar( int nTargetId, wModelTime &wTimeStart, wModelTime &wTimeShot );
	// panov }
	LandGroupState *setFollowingCurrentState();
	void setHumanLeader( bool bvalue ) {  
		if( movingImpl ) movingImpl->isLeaderHuman( bvalue );
	}
	bool getHumanLeader() {  
		if( movingImpl ) return movingImpl->isLeaderHuman();
		return false;
	}
	bool isCurrentState() {
		if( movingImpl ) return movingImpl->getCurrentState() ? true : false;
		return false;
	}

	void setFOPathAssignable(bool pathAssignable) {foPathAssignable = pathAssignable;}

protected:

	typedef				ed::vector<TaxiPoint> CarRoute;
	typedef				ed::vector<woCar *> ArmoredCars;

	typedef				ArmoredCars::iterator IterCarList;

	typedef				wActivity<wcColumn> wActivityColumn;
	wActivityManager	activities;
	wtPointer			activity_control;
	wtPointer			activity_assignTargets;
	wtPointer			activity_mailToHumanPlanes;
	wtPointer			activity_update;

    void                init(lua_State* meGroup_, unsigned char countryID_);

	void				control(wModelTime& timeStep);

	bool				mobility;
	bool				foTargetsAssignable;
	bool				foPathAssignable;
	ArmoredCars			carList;

	wiPath				mappedApprPath;

	void setUseLeader( bool bValue = true ) { _bUseLeader = bValue; }
	bool getUseLeader( ) { return _bUseLeader; }

private:
	LandGroup*		movingImpl;
	wcRoute *		route;
	AI::Mission *	pMissionTask;
	bool			taskActive;
	wcRoute::RouteType waypoints;

	bool	_bUseLeader;
    unsigned int     numberOfEnabledAI;

	struct NetData
	{
		NetData()
			:form(fNotDefined), routeSpeed(0.), state(gtNotDefined), path(0), 
			firstCarPathParam(0.), alarmState(ALARM_STATE_AUTO), roe(ROE_OPEN_FIRE)
		{
		}

		CarFormationType form;
		double routeSpeed;
		LandGroupStateType state;
		double firstCarPathParam;
		NM_ROAD::Road* path;
		GTMAlarmType alarmState;
		ROEType roe;
	};

	struct PPCache
	{
		PPCache()
			:pathParam(0.), path(0)
		{}

		double pathParam;
		NM_ROAD::Road* path;
	};
	mutable PPCache firstCarParCache;
	
	typedef ed::map<ObjectID, double>  OffsetsMap;
	static OffsetsMap offsetsFromBridges;
	static wcColumnMail mail;
	NetData netData;

	ColumnStatus prevStatus;
	wModelTime timeStatusChanged, timeFSRFChanged;

	bool isHumanColumn;

	// map<roadObject, pathParam>
	typedef ed::map<const lRoadDestroyedObject*, double> RoadObjects;
	RoadObjects _roadObjects;
	void updateRoadObjects(); //функция должна вызываться при смене пути
	bool sendBypassRouteRequest(double bridgeParam, ObjectID bridgeId);
	void cutRoute(double endPar);

	static wcColumn* netCreate(	ObjectID colId, NM_ROAD::Road* path, wcCoalitionName coalition, 
								bool targetsAssignable, unsigned int missionId, 
								const ed::string & name, const ColumnStatus& status);
	void netDestroy();
	void setNetStatus(const ColumnStatus& st);
	void setNetPath(PathQueryParams* pp);
	void setNetOrder(wcColumnMail::ColumnOrder order, int orderParam);
	void setFOTargetsAssignable(bool targetsAssignable) {foTargetsAssignable = targetsAssignable;}
	void updateCachedData();

	void update(wModelTime& timeStep); 
	ed::vector<osg::Vec2d> getFreeFormPos(const ed::vector<osg::Vec2d>& carsPos, 
		const ed::string& templId);


	friend class wcColumnMail;

#ifdef DEBUG_TEXT
	ed::string dbgTextOut;
#endif
};

inline wcColumn* columnById(ObjectID id)
{
	Registered *obj = RegisterManager::instance->Get(id);
	return dynamic_cast<wcColumn *>(obj);
}

#endif
