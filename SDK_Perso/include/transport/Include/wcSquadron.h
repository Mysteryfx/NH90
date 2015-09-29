//                    file wcSquadron.h
#ifndef __wcSquadron_h__
#define __wcSquadron_h__

#include "Transport.h"
#include "wControl.h"
#include "cPosition.h"
#include "wListener.h"
#include "wActivityManager.h"
#include "Common.h"
#include "weapons.h"
#include "wGroupTargetManager.h"
#include "Registry/Registered.h"
#include "wsFlightPlan.h"

#include "../../Frogfoot/Include/Aircrafts/wNetPathData.h"

const int MaxShipTargetNum = 100;

class  IwcCountry;
class wWorld;
class woShip;
class wcSquadron;

namespace AI
{
	class Mission;
}

#include <ed/vector.h>

#define IID_ISquadron Common::identify<wcSquadron>() 

class TRANSPORT_API wcSquadron : public wGroupTargetManager, public Registered
{         
public:

	typedef				ed::vector<TaxiPoint> ShipRoute;

    wcSquadron();
    ~wcSquadron();
	
	GET_DESC_METHOD(wcSquadron, wControl)

    static wcSquadron*	create(lua_State* meGroup_, IwcCountry* country_, bool master_, unsigned char countryID_);
    void				setCountry(lua_State* meGroup_, IwcCountry *pC, unsigned char countryID_, bool lateActivation = false);

	void				addMember(woShip* object);
	void				removeMember(woShip* object);

	void				SetTaskSquadron(int);
	float				getVelocitySquadron()	{return velocitySquadron;}
	cVector				getVectorStrivePoint()	{return vectorStrivePoint;}
	int					getRoutesize()			{return route.size();}
	ShipRoute*			getRoute()				{return &route;}
	void				leaderIsDead();
	void				updateVelocitySquadron();
	virtual int			GetLastWaypointId() const;
	//wControl interface begin
    virtual wModelTime  Control(); // функция, которую вызовет будильник. возвращает время, через которое проснуться в след раз
	wcCoalitionName		Coalition() const { return my_coalition; }
	wClass				Class() const { return wcClass_Squadron; }
	virtual bool		isDead() const;
	
	virtual Unit		getLeader() const;
	virtual Unit		getUnit(int number) const;
	virtual int			getSize() const;

	//void				setFromIndex(int fromWaypointIn) { fromWaypoint = fromWaypointIn; }
	void				setGotoIndex(int gotoWaypointIn);
	//int					getFromIndex() { return fromWaypoint; }
	int					getGotoIndex() { return goToWaypoint; }
	void				setRoute(const ShipRoute & routeIn);
	const cVector&		getGotoWaypoint();

    virtual void        destroy(bool immediately = false);
	//wControl interface end

	//AI::Controller interface
	virtual void		setOnOff(bool onOff);
	void				setOption( unsigned int name, const Option & value);	

    virtual LandGroupStateType state() const {return gtNotDefined;};

	virtual int			getLeaderId() { return idShipLeader; }
	virtual Unit		getUnitById( ObjectID id) const;
	virtual TaxiPoint*	getCurrentRoute();


	const AI::NetAIPath	&getNetRoute() const { return _netAIPath;  }
	void			setNetRoute( AI::Route *pRoute ) { _netAIPath.setRoute(pRoute); }
	void			setNetRouteIdx( int nIdx )	{ _netAIPath.iNetGotoIdx = nIdx; }
	int				getNetRouteIdx( )			{ return _netAIPath.iNetGotoIdx; }

	AI::NetAIControl &getNetControl() { return _netAIControl; }

	void			deleteUIRoute();
	void			addTargetTask( ObjectID targID, AI::Task *pTask );
	void			deleteTargetTask( ObjectID targID );

protected:

	void			_deleteTask( ObjectID nIdTask );
	void			_deleteTask( AI::Task *pTask );

	typedef				ed::vector<woShip*> Ships;

	typedef				wActivity<wcSquadron> wActivitySquadron;
	wActivityManager	activities;
	wtPointer			activity_control;
	wtPointer			activity_assignTargets;
	wtPointer			activity_mailToHumanPlanes;

    void                init(lua_State* config_, unsigned char coalitionID_);

	void				control(wModelTime& timeStep);

	void				deleteShips();    												
	void				informShipsAboutLeader(ObjectID idLeader);
    void				calculateElementMotionSquadron();

	AI::Mission *		pMissionTask;
	bool				taskActive;
	Ships				ships;
	int					numberActualPointRoute;
	bool				firstReferencingMotion;
	ShipRoute			route;
	//int					numberPointGoTo;
	float				velocitySquadron;
	float				velocitySquadronMax;
	cVector				vectorStrivePoint;
	ObjectID			idShipLeader;								

	int					fromWaypoint;
	int					goToWaypoint;
	cVector				vGotoWpt;

	AI::NetAIPath		_netAIPath;
	AI::NetAIControl	_netAIControl;
	ed::map<ObjectID,AI::Task*> _vMapTargTask;
};

#endif
