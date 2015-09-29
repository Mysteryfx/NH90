#ifndef __woATC__
#define __woATC__

/*
Air Traffic Controller (ATC) - the dispatcher that provides control over taxi, takeoff and landing operations for aircrafts.
ATC provides as most fast takeoff / landing rate as possible.
ATC provides safety.
Each object that can be a base for aircraft has ATC: airdromes, heliports, ships.
*/

#include "Flight.h"
#include "wMovingObject.h"
#include "wsFlightPlan.h"
#include "Comm/wCommunicator.h"
#include "Comm/wTransiverOwner.h"
#include "wControl.h"
#include "wResourceProvider.h"
#include "ATC_Messages.h"

#include "lTerraBasic.h"
#include "viGraphHelper.h"
#include "edTerrain/IRoadNetworkAdapter.h"

namespace NM_ROAD
{
    struct Crossroad;
    struct Road;
};

enum wMsg;
struct TaxiPoint;
struct TaxiRoute;
class IwoLA;
class wAirdrome;
class IwWarehouse;
class wAirbase;

struct Aircraft_Descriptor;

struct AirbaseInitState;

//Terminal - parking ramp

enum wsTermTypes
{
	wsttUnknown = 0,	// неизвестно что
	wsttTurnpoint,		// промежуточная точка маршрута
	wsttShelter,		// укрытие маленького самолета
	wsttBigShelter,		// укрытие большого самолета
	wsttOpenParking,	// открытая стоянка маленького самолета
	wsttBigOpenParking,	// открытая стоянка большого самолета
	wsttRunWayPoint,	// точка съезда с ВПП
	wsttHelicopter,	// точка съезда с ВПП
	wsttLastType		// последний тип
};

#define IID_IwsAdrTerminal Common::identify<wsAdrTerminal>()

namespace Lua
{
    class Config;
}

//Basic terminal
class FLIGHT_API wsBasicTerminal : public Common::FakeUnknown<Serializable>
{
public:
	wsBasicTerminal() : acquired_by(0),acquire_order(0),valid_spawn_time(-1){}
	virtual ~wsBasicTerminal();
    bool      is_free() const { return acquired_by == 0; }
    bool      is_free_for_me(ObjectID me) const;
    bool      is_free_for_me(const ed::string & miss_id) const;

	bool      is_acquired_by(ObjectID id) const  { return acquired_by == id;}
	bool      is_acquired_by(const ed::string & miss_id) const  { return acquired_by_miss_id == miss_id;}

    void      acquire(ObjectID id_);
    void      acquire(const ed::string & miss_id_);
    void      acquire(ObjectID id_, const ed::string & miss_id_);
    void      free() { acquire(0);}
    void      set_acquire_order(int ord);
    int       get_acquire_order() const  { return acquire_order;}
	ObjectID  get_client_id() const { return acquired_by; }
    woPointer get_acquired_object() const;
    const ed::string & get_acquired_miss_id() const { return acquired_by_miss_id; };
#ifdef DEBUG_TEXT
	const ed::string & get_acquired_object_desc() const;
#endif
    void serialize(Serializer &serializer)
    {
        serializer << acquired_by
                   << acquired_by_miss_id;
    }
    void      wait_for_objects_spawn(bool on_off);
    bool      owner_completely_dead_or_not_exist() const;
protected:
	ObjectID      acquired_by;
	ed::string   acquired_by_miss_id;
    int           acquire_order;
#ifdef DEBUG_TEXT
	ed::string	clientDesc;
#endif
    friend struct woAirMail;
public:
    viHelperPointer dbg_draw_object;
private:
    double valid_spawn_time;
};

//Client status for the ATC
enum ClientStatus
{
	NO_STATUS,
	WAITING,
	WAITING_FOR_STARTING,
	CLEARED_FOR_START_UP,
	STARTING,
	HOVER_CHECK,
	WAITING_FOR_TAXING_TO_RUNWAY,
	HOLDING_POSITION,
	CLEARED_FOR_TAXI_TO_RUNWAY,
	TAXING_TO_RUNWAY_ON_TAXIWAY,
	TAXING_TO_RUNWAY_ON_RUNWAY,
	WAITING_FOR_TAKEOFF,
	WAITING_FOR_TAKEOFF_ON_RUNWAY,
	CLEARED_FOR_TAKEOFF,
	TAKEOFF,
	DEPARTURE,
	INBOUND,
	WAITING_FOR_LANDING,
	CLEARED_FOR_LANDING,
	LANDING,
	ON_GLIDE_SLOPE,
	ROLLING,
	TAXING_FROM_RUNWAY_ON_RUNWAY,
	TAXING_FROM_RUNWAY_ON_TAXIWAY,
	PARKING,
	LAST_STATUS
};

class woATC;

//Client data for the ATC
struct FLIGHT_API ClientData
{
public:
	enum Type
	{
		TYPE_VOID = -1,
		TYPE_AIRPLANE,
		TYPE_HELICOPTER
	};
	ClientData();
	ClientData(ObjectID clientID);
	void		init(ObjectID clientID);
	void		setStatus(ClientStatus statusIn); //Sets status
	void		setHoverPoint(); //Sets hover point when control hovering is started
	bool		isDead() const;
	bool		In_Air_() const;
	woPointer	getPtr() const;
	const cPosition & Position() const;
	const cVector & getVelocity() const;
	bool		canTakeoff(wAirbase * airBase) const; //Checks if the aircraft is able to takeoff now
	bool		isEngineStarted() const; //Checks if engines are started
	ObjectID	getCurrentLandPlaceID() const; //Returns actual airbase where the client is
	void		send_message(woATC * pATC, wMsg msg); //Sends radio message to the client
	void		return_resources_to_warehouse(IwWarehouse * warehouse, bool returnSelf); // Returns client's aircraft, ammo and fuel to the airbase warehouse
	void		kill(); //Destroys the client

	const Aircraft_Descriptor * getAircraftDescriptor() const;
public:
	Type			type;
	ObjectID		ID;
	wControl *		group;
	int				N_obj;
	bool			human;
	ClientStatus	status;
	wModelTime		statusChangeTime;
	cPoint			hoverPoint;
	bool			beenInAir;
	mutable wModelTime	hoverStartTime;
#ifdef DEBUG_TEXT
	ed::string		clientDesc;
#endif
protected:
	inline IwoLA * getLA_() const;
	woPointer			ptr_;
	mutable bool		inAir_;
	mutable cPosition	pos_;
	mutable cVector		vel_;
};

typedef ed::map<ObjectID, ClientData> ClientsData;
struct ClientGroup
{
	typedef ed::list<ClientsData::iterator> Clients;
	Clients clients;
	float fuelRatioMin;
	float deltaLifeMax;
};
typedef ed::map<const wControl*, ClientGroup> ClientGroups;

struct GroupTaxi
{
	GroupTaxi()  {;}
	GroupTaxi(const wControl * groupIn);
#ifndef USE_TERRAIN4
	void add(ClientData * clientData, edterrain::RouteAdapter * road);
#else
	void add(ClientData * clientData, edterrain::IRoute* road);
#endif
	void remove(ClientData * clientData);
	void start(ClientData * clientData);
	void stop(ClientData * clientData);
	inline bool empty() const { return members.empty(); }
	float getHead() const;
	float getTail() const;
	float getMemberDist(const ClientData & clientData) const;
	struct Member
	{
#ifndef USE_TERRAIN4
		Member(ClientData * clientDataIn, edterrain::RouteAdapter * roadIn);
#else
		Member(ClientData * clientDataIn, edterrain::IRoute* roadIn);
#endif
		void start();
		void stop();
		inline bool isTaxing() const { return taxing; }
		inline bool isOnRamp() const { return onRamp; }
		float getDist() const;
		class Process
		{
		public:
			Process() { reset(); }
			inline void reset() { distAway = 0.0f; segmentNum = 0;}
#ifndef USE_TERRAIN4
			float getRemainDist(const ClientData & clientData, edterrain::RouteAdapter * road) const;
#else
			float getRemainDist(const ClientData & clientData, edterrain::IRoute* road) const;
#endif
		private:
			mutable cPoint	point;
			mutable float	distAway;
			mutable int		segmentNum;
		};
		bool			firstUpdate;
		bool			onRamp;
		bool			taxing;
		Process			process;
		ClientData *	clientData;
#ifndef USE_TERRAIN4
		edterrain::RouteAdapter * road;
#else
		edterrain::IRoute* road;
#endif
		float			pathLength;
	};
#ifdef _ED_HIDDEN_DEBUG 
	ed::string groupDesc;
#endif
	wModelTime requestTime;
	bool onRamp;
	unsigned int taxingMembers;
	typedef ed::vector<Member> Members;
	Members members;
	const wControl * group;
	mutable float lastMemberEqPathLength;
private:
	void calcLastMemberEqPathLength_() const;
};

typedef ed::map<const wControl*, GroupTaxi> GroupsTaxi;
typedef ed::deque<GroupsTaxi::iterator> GroupsTaxiStart;


struct LandingClient;

//Terminal for helicopters
class FLIGHT_API wsHAI_terminal : public wsBasicTerminal
{
public:
	wsHAI_terminal(){}
    dVector      takeoff_pnt;
    dVector      landing_pnt;
public:
	void serialize(Serializer &serializer)
	{
		wsBasicTerminal::serialize(serializer);
		serializer << takeoff_pnt
				   << landing_pnt;
	}
};

//Path on taxiways
struct road_info_
{
    road_info_()
    {
        start_   = 0;
        end_     = 0;
        for_big_ = 0;
    }
    int  start_;
    int  end_;
    bool for_big_;

    friend bool operator < (const road_info_ & item_1 ,const road_info_ & item_2)
    {
        if (item_1.start_ == item_2.start_)
        {
            if (item_1.end_ == item_2.end_)
                return item_1.for_big_ < item_2.for_big_;
            return item_1.end_ < item_2.end_;
        }
        return item_1.start_ < item_2.start_;
    };
};

#ifndef USE_TERRAIN4
typedef std::pair<road_info_,edterrain::RouteAdapter*> airdrome_road_with_info;
typedef ed::map <road_info_,edterrain::RouteAdapter *> airdrome_road_map;
#else
typedef std::pair<road_info_,edterrain::IRoute*> airdrome_road_with_info;
typedef ed::map <road_info_,edterrain::IRoute*> airdrome_road_map;
#endif

//Airplane terminal
class FLIGHT_API wsAdrTerminal : public wsBasicTerminal
{
public:
	wsAdrTerminal();
	~wsAdrTerminal();
public:
	int				Term_Index;		//Terminal index
	int				Term_Index_0;	//Index of initial point of the taxi path
	wsTermTypes		Term_Type;		//Terminal type
	cPoint			point;
	bool			TO_AC;			//true - taking off aircraft, false - landing aircraft
#ifndef USE_TERRAIN4
	edterrain::RouteAdapter* pRoad; //Taxi path
#else
	edterrain::IRoute* pRoad; //Taxi path
#endif
	void serialize(Serializer &serializer)
	{
		wsBasicTerminal::serialize(serializer);
		serializer << Term_Index_0
				   << (int&)Term_Type
				   << TO_AC;
	}
};

const float Time_of_Repair = 3600.0f;

#define IID_IwoATC Common::identify<woATC>()

typedef ed::list<wsAdrTerminal*> TerminalList;

class FLIGHT_API woATC :	public wCommunicator,
							public Common::FakeUnknown<Serializable>
{
public:
	class ResourceProvider : public wResourceProvider
	{
	public:
		enum { RESOURCE_ATC = 0};
		struct FLIGHT_API ATCCommonParams : public wResourceProvider::Params
		{
		public:
			typedef ed::vector<wResourceProvider::Params::ObjectModelNames> ATCObjectNamesByCountries;
		public:
			void load(Lua::Config & config);
		public:
			ATCObjectNamesByCountries								atc_object_names_by_countries;
		};
		static FLIGHT_API ATCCommonParams							atc_common_params;
		struct FLIGHT_API ATCAirdromeParams : public wResourceProvider::Params
		{
		public:
			ATCAirdromeParams();
			void load(Lua::Config & config);
		public:
			wResourceProvider::Params::ObjectModelNames	atc_object_names;
		};
		static ed::vector<ATCAirdromeParams>						atc_airdrome_params;
		inline bool													check_resource(ObjectID * objectID) const { return wResourceProvider::check_resource(RESOURCE_ATC, objectID); }
	private:
		virtual viFlag												is_objects_required() const;
		virtual const wResourceProvider::Params *					get_params() const;
		virtual const wResourceProvider::Params::ObjectModelNames *	get_model_names_by_resource(unsigned resource_name) const;
		virtual const unsigned int									get_resource_qty() const { return 1; }
#ifdef DEBUG_TEXT
		virtual ed::string											get_resource_name_str(unsigned char resource_name) const;
#endif
	};
	friend struct ClientData;

	//Listener handles world objects birth and death
	class Listener : public wListener
	{
	public:
		Listener() : pATC_(NULL) {;}
		Listener(woATC * pATC) : pATC_(pATC) {;}
		void onCreate(Registered * obj);
		void onDestroy(Registered * obj);
	private:
		woATC *pATC_;
	};

    airdrome_road_map road_map_;


#ifndef USE_TERRAIN4
	edterrain::RouteAdapter * get_road_(int start_,int end_,bool for_big_);
#else
	edterrain::IRoute* get_road_(int start_,int end_,bool for_big_);
#endif

public:

    woATC::woATC();
	virtual ~woATC();

	//Initialization
	void				setATC(wAirbase * P_, int callsign_);
	void				setATCComm(float frequency_, wModulation modulation_, int nNum);

    virtual void		serialize(Serializer &);

	//woATC update
	void				Control(void);

// MAIL {
	void				openDoor(ObjectID client);
	void				closeDoor(ObjectID client);
// } MAIL

	//Clients
	const ClientsData & getClients() const { return clients; }

	//Processes the message from the client
	void				onMessage(ObjectID client, ATCMessage message);

	//Returns true if the client have permission to land or takeoff
	bool				Glide_Slope(ObjectID id = 0) const;

	//Returns true if the client has permission to cross the runway (while taxing)
	bool				isCrossingPermit(ObjectID id = 0) const;

	//Returns the first client on the glide slope
	ObjectID			getGlideSlopeOccupant() const;

	//Returns true if the client has permission to taxi to takeoff
	bool				isTaxiToTakeoffPermit(ObjectID id) const;

	//Returns taxi path on ship from deck to parking ramp
    TaxiPoint*			Get_Taxi(ObjectID id);

	//Groups runway spawn routines
	bool				Get_BirthValid();
	void				Set_BirthValid(const wcPointer & client);
	void				Reset_BirthValid(const wcPointer & client);
	bool				Get_HelBirthValid();
	void				Set_HelBirthValid(const wcPointer & client);
	void				Reset_HelBirthValid(const wcPointer & client);

	//Returns landing point for the helicopter
	int					Get_Landing_Point(cPosition *Point, ObjectID client);

	//Returns takeoff point for the helicopter
	int					Get_TakeOff_Point(cPosition *Point, ObjectID client);

	//Free the client's terminal
	void				Clean_Taxi(ObjectID id_);

	//Returns if there are free landing pads for helicopters
	bool				Is_FreeHelLanding(ObjectID id) const;

#ifndef USE_TERRAIN4
	//Selects terminal and returns taxi path from the exit from the runway to the terminal
	edterrain::RouteAdapter*		Get_AdrLandingTaxi(ObjectID id_);
	//Restores taxi path from the exit from the runway to the terminal
	edterrain::RouteAdapter*		Get_RestoreLandingTaxi(ObjectID id_);
	//Selects terminal if required and returns taxi path from the terminal to the exit to the runway
	edterrain::RouteAdapter*		Get_AdrTakeOffTaxi(ObjectID id_);
	//Restores taxi path from the terminal to the exit to the runway
	edterrain::RouteAdapter*		Get_RestoreTakeOffTaxi(ObjectID id_);
	//Returns taxi path from current position to the exit to the runway
	edterrain::RouteAdapter*		Get_Curr_ToTakeOff(ObjectID id_);
	//Returns taxi path from current position to the terminal
	edterrain::RouteAdapter*		Get_Curr_ToTerminal(ObjectID id_);
	//Returns taxi path from current position to the given terminal
	edterrain::RouteAdapter*		Get_ToTerminal(ObjectID id_, int term_);
#else
	//Selects terminal and returns taxi path from the exit from the runway to the terminal
	edterrain::IRoute* Get_AdrLandingTaxi(ObjectID id_);
	//Restores taxi path from the exit from the runway to the terminal
	edterrain::IRoute* Get_RestoreLandingTaxi(ObjectID id_);
	//Selects terminal if required and returns taxi path from the terminal to the exit to the runway
	edterrain::IRoute* Get_AdrTakeOffTaxi(ObjectID id_);
	//Restores taxi path from the terminal to the exit to the runway
	edterrain::IRoute* Get_RestoreTakeOffTaxi(ObjectID id_);
	//Returns taxi path from current position to the exit to the runway
	edterrain::IRoute* Get_Curr_ToTakeOff(ObjectID id_);
	//Returns taxi path from current position to the terminal
	edterrain::IRoute* Get_Curr_ToTerminal(ObjectID id_);
	//Returns taxi path from current position to the given terminal
	edterrain::IRoute* Get_ToTerminal(ObjectID id_, int term_);
#endif

	//Returns if the runway is not damaged and free from clutter
	bool				isRunWayOpen() const;

	//Destruction time
	wModelTime			getTime_Dest()	{	return Time_Dest;}
	void				setTime_Dest(wModelTime t) { Time_Dest = t;}

	//HAI terminal operations
	void				selectAndAcquireHAITerminal(ObjectID client);
	void				freeHAITerminal(ObjectID client);
	int					getHAITerminal(ObjectID client) const;
	cPosition			getHAITermPos(int i) const;
	int					getTerminalByClient(ObjectID client);

	//Initial (native) coalition (from the mission)
	void				setInitCoalition(wcCoalitionName initCoalition_);
	inline wcCoalitionName	getInitCoalition() const { return initCoalition; }

	//Check if ATC tower is alive to provide radio communication
	bool				checkRadioResource() const;
	bool				checkRadio();

    void				onWorldSpawnComplete();

public:

	//Network
	virtual bool	netGetCreate(Mail::Message & msg) const;
// MAIL {
    void			procTerminalStatus(ObjectID	clientID, int Term_Index, int Term_Index_0, bool TO_AC);
	void			procHelTerminalStatus(int termindex, ObjectID clientID);
	void			procInitStatus(const AirbaseInitState & airbaseInitState);
	void			procClientMessage(ObjectID client, ATCMessage message);
	void			procSetClientStatus(ObjectID client, ClientStatus clientStatus);
	void			procSendRadioMessage(ObjectID client, wMsg msg);

	void			fillTerminalsState(TerminalsState & terminalsState, HelTerminalsState & helTerminalsState);
// } MAIL

	//wCommunicator interface
	void			processMessage(const wMessagePtr & message);
	virtual Interruption canInterrupt(const wMessagePtr & message1, const wMessagePtr & message2) const;
	virtual bool	canRemove(const wMessagePtr & message1, const wMessagePtr & message2) const;
	virtual bool	canDelay(const wMessagePtr & message1, const wMessagePtr & message2) const;
public:
    void	        transmitMessage(const wMessagePtr & message);

    static void     distribute_plane_gates();

	//Returns point of last turn, point of enter the glideslope
	cPoint			getT4(float distance, float altitude) const;
	cPoint			getT4() const;

	//Calculates time to enter and exit the critical area for the client during landing
	bool			getLandingClient(const ClientData & clientData, LandingClient & landingClient);

#ifdef DEBUG_TEXT
	//Debug text
	ed::string		debug_text() const;
#endif

protected:

	//Initialization
	void			Init0();
	void			initCommunicator(int nNum, float fFreq);
	ed::string		getOwnerName() const;

	//Initialize clients by the message from the server ATC (client)
	void			initClientStatus(ObjectID clientID, const ClientInitState & clientInitData);

	//Terminals initialization
	void			Init_Terminals(void);
	void			AddTerminalPoint(int, wsTermTypes);
	void            initialize_HAI_terms();

	//Aircraft distribution by information from the mission
	static int      distribute_coalition(Lua::Config & config);
	static int      distribute_group(Lua::Config & config,int order);
	int             distribute_unit(Lua::Config & config,int start_pnt,bool hai_term,int order);
	int 			distribute_unit_into_hai_term(const ed::string & str);

	//Update status
	void			updateTransmitterPos() const;
	void			updateCoalition();
	void			updateTermObstructions();
	void            controlRunwayOpen();

	//Control procedures
	void			controlClientsStatus();
	void			controlClientGroups();
	void			controlTakeoffAndLanding();
	void			controlTakeoff();
	void			controlLanding(wModelTime & timeToFirstRunwayEnter, unsigned int & arrivingOnRunwayClientsCount);
	void			controlToTaxiToRunway();
	int 			Control_Airdrome_Door(ObjectID, bool);
	void			controlSpotLights();

	//Operations with terminals

	//Operations with helicopter terminals
	bool            acquireHAITerm_(int i, ObjectID client);
	int				getHAITerminal_(ObjectID client) const;
	void            freeHAITerminal_(int i);
	int             get_free_HAI_term() const;
	void			checkFactTerminal(const ClientData & clientData);

	//Cleans terminals by aircraft type
	void			Clean_Terminals(unsigned char tc);

	//Client routines
	ClientData &	getClient(ObjectID clientID);
	virtual void	onMessage(ClientData & clientData, ATCMessage message);
	void			sendMessage(wMsg msg, MovingObject * mo);

	//Sets client status local (server)
	virtual void	setClientStatus(ClientData & clientData, ClientStatus clientStatus, wMsg msg);
	void			setClientStatusServer(ClientData & clientData, ClientStatus clientStatus, wMsg msg);

	//Sets client status remote (client)
	void			setClientStatusClient(ClientData & clientData, ClientStatus clientStatus);

	//Remove client from the list
	void			freeClient(ClientData & clientData);
	ClientsData::iterator freeClient(ClientsData::iterator it);


	//wListener callbacks
	void			onObjectCreate(ObjectID objectID);
	void			onObjectDestroy(ObjectID objectID);

	//Checks

	//Check client type
	bool            valid_terminal_type(wsAdrTerminal * term,const wsType & type,bool human) const;
	bool            is_BigAC(const wsType & type) const;

	//Check terminal obstruction
	bool			isAdrTermObstructedFor(const wsAdrTerminal & term, const woPointer & pretendent) const;
	bool			isHAITermObstructedFor(int index, const woPointer & pretendent) const;
	bool			isPointObstructedFor(const cPoint & point, const woPointer & pretendent) const;

	//Returns if the runway free from clutter, not from clients
	bool			isRunwayFree() const;
	bool			isRunwayFreeFor(ClientData & clientData) const;

	//Checks if there are object of a given size
	bool			IsHereOutsider(cPoint v_pos, bool big);

	//Retrieve data

	//Returns index of the point that is the exit to the runway
	int				Get_TO_RW_Point(void);

	//Returns index of the point that is the exit from the runway
	int				Get_Landing_RW_Point(void);

	//Returns the client velocity relative the parent velocity
	cVector			getClientVelocity(const ClientData & clientData) const;

	bool			BirthValid; //If the runway is ready for airplane group spawn on it
	bool			HelBirthValid;  //If the runway is ready for helicopter group spawn on it
	bool			FreeHelLanding; //It there are free landing pads for helicopters

    wModelTime		Time_Dest;  //Time of runway damage
	bool			runwayIsObstructed; //Runway obstruction flag
	GroupsTaxi		groupsTaxi; //Taxi structures by groups. Taxi to runway.
	GroupsTaxiStart groupsTaxiStart; //Taxi structures of groups which are waiting for permission to taxi to runway
	ed::set<ObjectID> GSpermissions; //List of clients, which have permission to land or takeoff
	unsigned int	crossingBlockersCount;	 //Counter of clients which presence can deny other clients to cross the runway while taxing
	bool			spotLightsOn;
	unsigned int	landingAndWaitingForLandingClientsCounter;
	unsigned int	waitingForTakeOffClientsCounter;
	unsigned int	takingOffClientsCounter;

    ed::vector<wsHAI_terminal> HAI_terms;
	int				N_L_HAI;    //How many helicopters are landed
    unsigned char	Max_Term;   //Total count of airplane terminals
	TerminalList	wsTerm_list;//Airplane terminals

	wAirbase *		pParent;	//Airbase: airdrome, heliport, ship
	wcCoalitionName	initCoalition; //Native coalition
	wModelTime		lastUpdateCoalitionCall;
	wsType			P_obj_type;
	ObjectID		ParentID;
	bool			it0;		// true - taxiways are initialized
	bool			first_update;

	mutable unsigned int communicatorPlaceID;
    ed::vector<wTransiverOwner*> m_arpTransiverOwners;

	ClientsInitState clientsInitState; //clients initial state (client)
	ClientsData		clients; //Client map
	ClientGroups	clientGroups; //References to elements in the map above combined in groups

	Listener		listener;
	ResourceProvider resourceProvider;

	friend struct woAirMail;
	static woAirMail mail;
private:
    wcPointer plane_birth_client_;
    wcPointer helli_birth_client_;

    wAirdrome *		get_parent_as_airdrome();
    const wAirdrome * get_parent_as_airdrome() const;

    int             acquireFreeTerm_(ObjectID client,bool to_ac = false);
    wsAdrTerminal * getTerminalByTermIndex_(int term_index);
    wsAdrTerminal * getTerminalByClient_(ObjectID client,bool get_first_free = false);
    wsAdrTerminal * getTerminalByClient_(ObjectID client,const ed::string & miss_id,bool get_first_free = false);

	void			return_object_from_terminal_to_warehouse_and_free(wsBasicTerminal & term,IwWarehouse * warehouse);
};

#endif __woATC__
