#ifndef __wAirbase__
#define __wAirbase__

/*
Airbase can be a base for airplane and/or helicopters: airdrome, heliport, ship.
Airbase has Air Traffic Controller (ATC).
*/

#include "Flight.h"
#include "wMovingObject.h"
#include "wGroundCrew.h"
#include "ATC_Messages.h"
#include "wListener.h"

class woATC;
class cPosition;
enum ClientStatus;
enum wModulation;
struct lua_State;
struct RunWay;
struct TaxiRoute;

struct AirbaseInitState
{
	wcCoalitionName		initialCoalition;
	wcCoalitionName		coalition;
	wModelTime			timeDest;
	ClientsInitState	clients;
	TerminalsState		terminals;
	HelTerminalsState	helTerminals;
};

class FLIGHT_API wAirbase : public MovingObject, public wGroundCrewOwner
{
protected:
    WorldSpawnDelegate<wAirbase> * spawn_delegate_;
    virtual void create_spawn_delegation();
public:
    wAirbase():spawn_delegate_(0), m_pATC(nullptr)
    {
        groundCrewResourceProvider.set_object(this);
    }
    virtual ~wAirbase();
	virtual wCommunicator *		getCommunicator();
	//Coalition and stat routines
	virtual	void				setCoalition(const wcCoalitionName coalition);
	virtual	void				setInitCoalition(const wcCoalitionName coalition);
	virtual	void				setState(const unsigned char state);

	//Processes the message from the client
	virtual void				onMessage(ObjectID client, ATCMessage message);
	//
	//Returns landing point for the helicopter
	virtual int					Get_Landing_Point(cPosition *Point, ObjectID client);

	//Returns takeoff point for the helicopter
	virtual int					Get_TakeOff_Point(cPosition *Point, ObjectID client);
	//
	//Returns position of the runway
	virtual const cPosition &	GetRunwayPosition(int iRunWay = 0) const = 0;
	//
	virtual bool				Is_FreeHelLanding(ObjectID id) const;
	
	//Return helicopter terminals data
	virtual int					getHAITerminal(ObjectID id);
	virtual cPosition			getHAITermPos(int i) const;
	
	//Returns true if the client have permission to land or takeoff
	virtual bool				Glide_Slope(ObjectID id = 0) const;
	
	//Returns if the runway is not damaged and free from clutter
	virtual bool				isRunWayOpen() const;
	virtual cPoint				getOrbit(ObjectID id) const;
	//
	virtual void				AddATC(int wished_callsign_id);
	virtual void				AddATCComm(float frequency, wModulation modulation, int nNumStation);
    virtual woATC*				getATC() const { return m_pATC; };
	// MAIL {
	friend struct woAirMail;
	virtual bool				netGetCreate(Mail::Message&) const;
	void						procInitStatus(const AirbaseInitState & airbaseInitState);
	void						procClientMessage(ObjectID clientID, ATCMessage message);
	void						procSetClientStatus(ObjectID clientID, ClientStatus status);
	void						procSendRadioMessage(ObjectID clientID, wMsg msg);
	void						procSetCoalition(wcCoalitionName coalition);
	void						procSetInitCoalition(wcCoalitionName coalition);
	void						procHelTerminalStatus(int termIndex, ObjectID clientID);
	// } MAIL

	//Returns runway for landing
	virtual const  RunWay*		Get_LandingRW()  const { return nullptr; }

	//Returns runway for takeoff
	virtual const  RunWay*		Get_TakeOff_RW() const { return nullptr; }

	//WTF?! Why specific ship functions are here?!
	
	//Return taxi path for ship
	virtual TaxiRoute	*		Get_Taxis()		{ return nullptr; }	

	//Return ship landing point for helicopters
	virtual cVector				Get_Landing_Point();
private:
	virtual MovingObject*		getObject();
protected:
    virtual  void               onWorldSpawnComplete() {};
	virtual  void				create_warehouse(const ed::string & mission_id_);

	woATC*	m_pATC;	//”казатель на диспетчера.
};

typedef cPointerTemplate<wAirbase>	woAirbasePointer;


#endif //__wAirbase__
