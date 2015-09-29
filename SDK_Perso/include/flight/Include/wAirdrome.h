//                    file wAirdrome.h
#pragma once

#include "Flight.h"
#include "wAirbase.h"
#include "wMovingObject.h"
#include "LandObjects\lSiteElement.h"
#include "lTerraBasic.h"
#include "viSearch.h"
#include "edTerrain/IRoadNetworkAdapter.h"

enum wModulation;

namespace path 
{
    class PathNetwork;
};
namespace NM_ROAD
{
    struct Crossroad;
    struct Road;
};

// Привязка к квадратам
struct PredVector
{
    bool operator ()(const cVector& v1, const cVector& v2) const
    {
        if(v1.x<v2.x) return true;
        if(v1.x>v2.x) return false;
        return v1.z<v2.z;
    }
};

class woATC;
struct ClientData;
class woRunWay;
class lLandObject;
class lObjWithLights;
class IwRadioBeacon;
class wAirdrome;
class lLandAirfieldObject;
class RoadnetworkFile;

#include "Randomizer\RandomObject.h"

struct RunWay      // Описывает взлетно-посадочную полосу (ВПП). 
{  
	int			Number_Vpp; // Номер(Имя) ВПП.
	int			Name_Vpp;   // Курс ВПП в десятках градусов.
	int			Side_Vpp;	// Left/Right - для двухполосных аэродромов
	cVector     R_Vpp;   // Координаты радиуса-вектора центра ВПП:x,y,z.
	double      Psi_Vpp;    // Курс ВПП (град)
	double      Length_Vpp;	// Длина ВПП без полос безопасности.
	double      Width_Vpp;  // ширина ВПП
	/// Holds argument to drive PAPI/VASI lights. Invalid value is -1.
	int 		alsArgument;
	// glide path angles
	double		lowGlidePath;
	double		slightlyLowGlidePath;
	double		onLowerGlidePath;
	double		onUpperGlidePath;
	double		slightlyHighGlidePath;
	double		highGlidePath;
	//navaids
	bool ILS, PRMG, outerMarker, middleMarker;
	cPoint ILSpos, outerMarkerPos, middleMarkerPos;

	// FLIGHT_API RunWay();

	FLIGHT_API friend Serializer& operator << (Serializer& serializer, RunWay& data);
	FLIGHT_API RunWay &operator = (const RunWay& rw);
};                         


// На аэродроме
struct airfield_placement_data
{
#ifndef USE_TERRAIN4
    edterrain::RoadNetworkAdapter    *   roadnet_file;
    ed::map<edterrain::CrossroadAdapter*,ObjectID>   shelters;
#else
	edterrain::IRoadNetwork* roadnet_file;
    ed::map<edterrain::ICrossroad*, ObjectID> shelters;
#endif
    ed::list<ObjectID>      AirfieldCameraDeclareList;  // Список камер на аэродроме
    cPosition                default_camera_position;
    void  InitSceneObjects();
     airfield_placement_data();
    ~airfield_placement_data();
};


class FLIGHT_API wAirdromeActivity : public wTime
{
public:
	wAirdromeActivity();
	virtual wAirdromeActivity::~wAirdromeActivity();
	void NextEvent();
	wAirdrome*	obj;
	bool		first_update;
};


class lShelterHandler
{
public:
    // Открывающиеся или закрывающиеся двери в настоящее время
    struct ShelterActivate 
    {
        ObjectID shelter;
        bool     typeAction;	//true -дверь открываем, false-закрываем
    };
	typedef ed::list<ShelterActivate> tag_shelterActivateList;
    wTime * activity;
    tag_shelterActivateList            shelterActivateList;
public:
    lShelterHandler() { activity = NULL;}
    ~lShelterHandler();
    wModelTime ShelterOperation();									    // Процесс Открывания( закрываем) двери  укрытий на аэродромах
    void       StartShelterDoorOperation(ObjectID shelter, bool bOpen); // Начать открывание/закрывание
};

typedef std::pair<IwRadioBeacon*, int> wRunwayBeacon;
typedef ed::map<unsigned char,ed::string> name_map;

struct ShelterState
{
	ObjectID	id;
	bool		state;
};

typedef ed::vector<ShelterState> SheltersStatus;

struct AirdromeInitState : public AirbaseInitState
{
	SheltersStatus		sheltersStatus;
};

#define IID_IAirdrome Common::identify<wAirdrome>() 

class FLIGHT_API wAirdrome :	public wAirbase,
								public lSite
{     
    lShelterHandler theShelterManager;

    ed::string en_name;
    ed::string ru_name;
    airfield_placement_data                    placement_;

    static ed::vector<wAirdrome *>            airdromes;
    static RandomObject randomEvenly; // равномерное (0, 1)
    static void                                distribute_group();

public:
	typedef ed::map<ObjectID, bool> ShelterToggleList;
    static void                                spawn();
    static void                                clear();
    static lua_State                         * getConfig();;
    static int                                 getAirdromeIndex(wAirdrome * adr);                
    static wAirdrome *                         getAirdrome(unsigned i);                
    static wAirdrome *                         getAirdromeByLev4(unsigned char lev4); 
    static unsigned                            getAirdromeCount() { return  airdromes.size(); }
    static const ed::string                 & getAirdromeName(unsigned char lev4,bool russian = false);
    static void                                Create(int Name,Lua::Config &config);

    wAirdrome(); 
    virtual ~wAirdrome();

	const cPosition& GetRunwayPosition(int iRunWay = 0) const;
	const RunWay*	getLongestRunway() const;
	
    int				Class()	{return woClass_Airdrome;}
	const float		Course() {return m_fPsi;}
	void			serialize(Serializer &serializer);
	virtual void	PostCollisionProcessing(wsCollisionData*, ObjectID);
	const float		Hrad(void);
	virtual	void	setCoalition(const wcCoalitionName coalition);
	virtual	void	setState(const unsigned char state);

	void			Control(); 

    int				On_RunWay(const cPoint & point, int runway_num = 0) const;// true - точка на ВПП
	bool			isAirdromeRadarValid();	
    const ed::string & getName(bool russian = false) const { return russian ? ru_name : en_name; };
    const ed::string & get_ICAO_Code() const { return icao_code; };
    void			add_beacon(Lua::Config & config, int RWS_ID, bool attachToRunway);
	unsigned int	GetRunWayID(int n_rw);
    unsigned        GetRunWaysCount() const { return N_RW; } 
	virtual const RunWay* Get_LandingRW() const;// Возвращает указатель на ВПП посадки

#ifdef USE_TERRAIN4
	edterrain::IRoadNetwork* getTaxiways(){return placement_.roadnet_file;}
#endif

#ifndef USE_TERRAIN4
	edterrain::RouteAdapter*			Get_AdrLandingTaxi    (unsigned int id_); 
	edterrain::RouteAdapter*			Get_AdrTakeOffTaxi    (ObjectID id_); 
	edterrain::RouteAdapter*			Get_RestoreTakeOffTaxi(unsigned int id_); 
	edterrain::RouteAdapter*			Get_RestoreLandingTaxi(unsigned int id_); 
	edterrain::RouteAdapter*			Get_Curr_ToTakeOff    (unsigned int id_);
	edterrain::RouteAdapter*			Get_Curr_ToTerminal   (unsigned int id_);
	edterrain::RouteAdapter*			Get_ToTerminal		  (ObjectID id_, int term_);
    edterrain::RouteAdapter*			GetPathOnAirfield     (int crossroad_from, int crossroad_to, bool isBigAircraft);
    edterrain::RouteAdapter*			GetRoadOnAirfield(unsigned road_index);
    edterrain::CrossroadAdapter *    GetCrossRoadOnAirfield(unsigned cross_index);
#else
	edterrain::IRoute*					Get_AdrLandingTaxi    (unsigned int id_); 
	edterrain::IRoute*					Get_AdrTakeOffTaxi    (ObjectID id_); 
	edterrain::IRoute*					Get_RestoreTakeOffTaxi(unsigned int id_); 
	edterrain::IRoute*					Get_RestoreLandingTaxi(unsigned int id_); 
	edterrain::IRoute*					Get_Curr_ToTakeOff    (unsigned int id_);
	edterrain::IRoute*					Get_Curr_ToTerminal   (unsigned int id_);
	edterrain::IRoute*					Get_ToTerminal		  (ObjectID id_, int term_);
    edterrain::IRoute*					GetPathOnAirfield     (int crossroad_from, int crossroad_to, bool isBigAircraft);
    edterrain::IRoute*					GetRoadOnAirfield(unsigned road_index);
    edterrain::ICrossroad*				GetCrossRoadOnAirfield(unsigned cross_index);
#endif

    unsigned        GetNumCrossRoadsOnAirfield();
    bool            GetAirfieldCrossPoint (unsigned cross_index,cVector *pt);

    void			Clean_Taxi(unsigned int id_); // Очистить рулежку
	int				Get_RW_Number(bool) const;// на входе true для взлета, выход - номер ВПП 
	int				Get_RW_Course_Number(bool toFlag);
	const RunWay*	Get_TakeOff_RW() const;// Возвращает указатель на ВПП взлета
	bool			isRunWayOpen()  const;
	const RunWay*	getAirdromeRWs() const	{return A_RWs;}
	woRunWay*		getFirstAliveRunWay();
	void			OnOffProjector(bool param); // Оn/Off прожекторы param=[0, 1]
	RunWay*			Get_ClosestRW(float,const cVector&); // дать ближайшую ВПП
	bool			isRunwayFree(viSearchHandler searchHandler, void * data, viFlag flags) const;
	cPosition		getRWPos(const RunWay * RW) const;
	float			distToRunway(const RunWay * RW, const cPoint & point);
	bool			isCrossingRunway(const RunWay * RW, const cPoint & from, const cPoint & to);
	double          getIlsFrequency(int RunwaySide = -1);  // returns 0 if no ILS
	double          getTacanChannel();  // returns 0 if no TACAN
	inline const ShelterToggleList & getShelterToggleList() const { return shelterToggleList; }

    cPosition       getDefaultCameraPosition() const;
	
	// очищает аэродром от кратеров разбитых самолётов и чинит его
	typedef			ed::vector<viObject*> viObjectList;
	static bool		viDeleteHandler(viObject *item, viSearch *search, void *data);
	void			CleanAndRepair();

	// интерфейс lSite
	virtual float	get_param_value(unsigned int param_num) {return 0.0f;}
	virtual void	clear_site(bool full = false);
	virtual void	on_element_disable(lSiteElement*);
    
#ifdef DEBUG_TEXT
	virtual const char * debug_text();
#endif
	
	bool            isMilitary() const  { return is_military;};

    int             openTermDoor(int term_index,bool action);
private:

    void			Init(int Name,Lua::Config &config); // инициализация
    void    		GetAirdromePlaceData(int Name,Lua::Config & config); 
	void			createAirdromeActivity();
	void			Control_Light(void);// Вкл/выкл посадочных огней
	void			Control_Radar_Mail(void);// Почта об облучении гуманоиду.
	void			Control_Beacons(void);// Вкл/выкл приводных маяков

	bool			Check_RunWayHit(const cPoint & point, int n) const;
	void			InitRunWayData(void);
	void			CreateRunWay();	
	void			DeleteRunWay(int i = 0);
	int				formVPPName(float _psi);
	woRunWay*		AddRunWay(cPosition& _pos, float _psi, float len, wsType _type);
	//радио
	void			SetATC(Lua::Config & config);
	void			readApproachLightsConfig(Lua::Config & config);
	void			create_communications(Lua::Config & config);
	void            clear_communications();
	int				get_RW_data_by_name(const ed::string & name);	

protected:
	// Andreev : ICAO code
	ed::string icao_code;
	
	/// Holds pointer to new airfield.
	/// This object will not be delete in destructor.
	lLandAirfieldObject *_airfield;

	/// Tries to get graphics arifield representation.
	void getGraphAirfied();

	// MITKA заполнить список прожекторов
	void			FillProjectorList(const cVector& landingpoint);

            // Cохранить
	bool				FirstRun;
	wModelTime			Time3;
	wModelTime			Time_Light;
	wModelTime			Time_Beacons;
	int					N_RW;		// Количество woRunWay
    float				m_fPsi;		// курс аэродрома	
	cVector				R_lf[2];	// Данные для проверки попадания в ВПП
	cVector				nx_lf[2];	// Данные для проверки попадания в ВПП
	cVector				nz_lf[2];	// Данные для проверки попадания в ВПП
			// Восстановить
    RunWay				A_RWs[4];		// Указатель на массив математических ВПП
	lLandObject			*pSP[4];		// Указатели на машины системы посадки
	lLandObject			*pAdrRadar;
	// MITKA
	ed::list<lObjWithLights*> projectors;		// Указатели на машины подсветки точки посадки

	ed::list<wRunwayBeacon>	radio_communications;

	wtPointer				pAirdromeActivity; 
        // Не нуждаются в сохранении и восстановлении
	woRunWay*			Arr_RunWay[2];	// woRunWay pointers
	bool				C_Light;	

    bool                is_military;
    bool                single_directional;
    int                 single_directional_value;
	int					defaultRunway;
	ShelterToggleList	shelterToggleList;

    void                onWorldSpawnComplete();
    void                onFirstRun();
#ifdef DEBUG_TEXT
	char				debugTextStr[15000];
#endif

	void				create_warehouse(const ed::string & mission_id_);


// MAIL {
	friend struct woAirMail;
	void		doOpenDoor(unsigned n_term_index)  { openTermDoor(n_term_index,true);};
	void		doCloseDoor(unsigned n_term_index) { openTermDoor(n_term_index,false);};
	void		doDamage(float modelTime, unsigned short damage);
    void        procTerminalStatus(ObjectID	clientID, int Term_Index, int Term_Index_0, bool TO_AC);
	void		procInitStatus(const AirdromeInitState & airdromeInitState);
// } MAIL
};




