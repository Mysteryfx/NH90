#ifndef __IwWorld_h__
#define __IwWorld_h__

#include "WorldGeneral.h"
#include "wsType.h"
//#include "wMovingObject.h"
#include "IwcCountry.h"

class wcSquadron;
class wcColumn;
struct wsInitData;
class lLandObject;
class wRadioManager;
class wsZonesStorage;
class Parameter;
enum wcCoalitionName;
enum CountryService;
struct wcAWACS_target;

struct lua_State;

#include "../../Inter/Include/IWorldNet.h"

class wAirbase;
typedef ed::list<wAirbase*> AirBases;
typedef ed::map<ObjectID, unsigned int> ExploredUnits;

class IwWorld
{
public:
	virtual			~IwWorld() = 0 {};
	virtual int				Init() = 0;
	virtual void			Start() = 0;
	virtual void			Finish() = 0;
    virtual void			create(const Common::Identifier &identifier, Common::Identifiable **identifiable) = 0;
	virtual void			onShoot(wcCoalitionName coalition, ObjectID shooterID, const wsType & weaponType, void * weaponData, ObjectID weaponID, ObjectID targetID, const cPoint & targetPoint, wModelTime impactTime = 0.0) = 0;
	virtual void			onDamage(wcCoalitionName coalition, ObjectID shooterID, ObjectID weaponID, ObjectID targetID, float deltaLife) = 0;
	virtual bool			detectMissile(woPointer pMissile, bool missileIsRadiating) = 0;
    virtual bool	        process(wModelTime) = 0;
    virtual bool			process(unsigned int requestedTime) = 0;
	virtual void			NextFrame(wModelTime) = 0;	// Моделирование между кадрами

    virtual ObjectID		GetNearestBase(const cPoint & point, unsigned int flags) const = 0;
	virtual void			ProcessCommands(int) = 0;
	virtual void			ProcessCommand(int, float) = 0;
    virtual void            Clean_World() = 0; // очистить мир и будильник
	virtual woPointer       Get_woPointer(ObjectID id) = 0; 
	virtual woPointer		HumanPtr() = 0;
	virtual void			SetHuman(woPointer p) = 0;
	virtual void			SetCockpitNyMove(bool on) = 0;
	virtual woPointer		GetGroundUnitControlledByHuman() = 0;
    virtual void			misInitWorld(lua_State*) = 0;
    virtual void			misSpawnObjects(lua_State *) = 0;
    virtual void			misSpawnLocalPlayer(lua_State *) = 0;
	virtual void			misPrecache() = 0;
    virtual int				setWeather( lua_State* config_, double startTime) = 0;
    virtual void            preloadResourcesForUnit(lua_State * config_,const ed::string & name) = 0;
	// --- ОРУЖИЕ ----------------------------------------------------------
	virtual void			serialize(Serializer &) = 0;
	virtual bool			get_restoreW() = 0;
	virtual bool			GetWorldClean() = 0;
	virtual					wRadioManager *GetRadio() = 0;
	virtual wsZonesStorage&	getZonesStorage() = 0;
	virtual IwcCountry *	GetCoalition(wcCoalitionName coalition) = 0;

	//
	virtual void setLocalUnitID(const ed::string&) = 0;
	virtual const ed::string& getLocalUnitID() = 0;

	virtual void setLocalCoalition(wcCoalitionName coalitionID) = 0;
	virtual void setLocalRole(Role roleID) = 0;

	virtual wcCoalitionName localCoalition() const = 0;
	virtual Role localRole() const = 0;
	virtual bool isPilotControlVehicles() const = 0;

	virtual void registerUnit(woPointer pUnit) = 0;
	virtual void unregisterUnit(woPointer pUnit) = 0;
	virtual void unregisterUnit(const ed::string & name) = 0;
	virtual woPointer getUnitByName(const ed::string & name) = 0;

	// OBSOLETE
	bool	netIsRunning() const { return globalWorldNet->netIsRunning(); }
	bool	netIsServer() const { return globalWorldNet->netIsServer(); }
	ObjectID netServerID() const { return globalWorldNet->netServerID(); }
	ObjectID netLocalID() const { return globalWorldNet->netLocalID(); }

	virtual unsigned		getRandomObjectsOrigin() const = 0;

	virtual bool			getActivePauseState() const = 0;
	virtual void			setActivePauseState(bool FlagIn) = 0;
	virtual void			cycleActivePauseState() = 0;

	virtual void			addBase(wAirbase * pAirBase) = 0;
	virtual void			removeBase(wAirbase * pAirBase) = 0;
	virtual const AirBases& getAirBases() const = 0;
	virtual ObjectID		getAirdromeIDByName(int name) = 0;

	virtual void			toggleServiceProvider(wcCoalitionName coalition, CountryService countryService, ObjectID objectID, bool on) = 0;
	
	virtual void			missionScriptDoFile(const ed::string & fileName, int argCount = 0) = 0;
	virtual void			missionScriptDoString(const ed::string & str, int argCount = 0) = 0;
	virtual bool			missionScriptDoStringPred(const ed::string & str, int argCount = 0) = 0;
	virtual void			missionScriptCallFunc(const char * path, int argCount = 0) = 0;
	virtual lua_State *		missionScriptGetState() = 0;

	virtual void			addExploredUnitRef(wcCoalitionName clt, ObjectID id, ObjectID explorerId = 0) = 0;
	virtual void			removeExploredUnitRef(wcCoalitionName clt, ObjectID id, ObjectID explorerId = 0) = 0;
	virtual void			removeExploredUnits(ObjectID explorerId) = 0;
	virtual const ExploredUnits& exploredUnits(wcCoalitionName clt) = 0;

	virtual void			addAWACSTarget(wcCoalitionName coalition, ObjectID id, const wcAWACS_target & target) = 0;
	virtual void			updateAWACSTarget(wcCoalitionName coalition, ObjectID id, const wcAWACS_target & target) = 0;
	virtual void			removeAWACSTarget(wcCoalitionName coalition, ObjectID id) = 0;

	virtual void			takeUnitControl(unsigned ID) = 0;
	virtual void			dbgExplosion (const dVector & point,
										  const dVector & direction,
										  float param1 = 0,
										  float param2 = 0,
										  const char * shell_name = 0) = 0;
    virtual void            validatePointerOnObjectID(ObjectID ID_, woPointer woPnt) = 0;
	virtual void			immediateFreeUnit() = 0;//without sending message , only in case other messages handlers
	virtual void			immediateTakeUnitControl(unsigned ID) = 0;//without sending message , only in case other messages handlers

	virtual const char *    checkUnitForControl(woPointer  ptr)= 0; //will return error string;
}; // class IwWorld

extern  WORLDGENERAL_API IwWorld* globalWorld;

#endif
