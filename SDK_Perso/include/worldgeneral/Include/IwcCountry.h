#pragma once

struct lua_State;
class MovingObject;
class wTAC;
class wcAWACS;
class wCommunicator;
class wMessage;

#include "CoalitionInfo.h"
#include "wControl.h"
#include "RefPoint.h"
#include <ed/list.h>
#include "ICargoMan.h"
#include "LandObjects\lLandObject.h"


//Service of coalition
enum CountryService
{
	COALITION_SERVICE_ATC,
	COALITION_SERVICE_AWACS,
	COALITION_SERVICE_TANKER,
	COALITION_SERVICE_FAC,
	COALITION_SERVICE_MAX
};

typedef ed::list<ObjectID> ServiceProviders;

//Target detected by AWACS
struct wcAWACS_target
{
	wModelTime		last_update;
	wcCoalitionName	country;
	cPosition		position;
	cVector			velocity;
	int				spike_count;
	float			course;
	float			reflection;
	wsType			type;
#ifdef _ED_HIDDEN_DEBUG 
	ed::string		targetDesc;
#endif
	bool			Fifty_NM_Override;
	bool			Five_NM_Override;
	bool			TakeOff_event;
	friend 	Serializer& operator << (Serializer& serializer,wcAWACS_target& data);
};

//Target detected by AWACSes
struct AWACSTargetData
{
	AWACSTargetData();
	wcAWACS_target target;
	//unsigned int refCount;
};


typedef ed::map<ObjectID, AWACSTargetData> AWACSTargetDataMap;

inline bool findTarget(const AWACSTargetDataMap & awacsTargetDataMap, ObjectID id)
{
	return awacsTargetDataMap.find(id) != awacsTargetDataMap.end();
}

typedef ed::list<ObjectID> Players;

typedef ed::list<wControl*> CountryMembers;


class WORLDGENERAL_API IwcCountry
{
public:
	virtual ~IwcCountry() = 0;

	//Group management
	virtual void				addCountryMember(const wcPointer & pMember) = 0;
	virtual void				removeCountryMember(const wcPointer & pMember) = 0;
	virtual wControl *			addGroup(lua_State* config, unsigned char countryID) = 0;
	virtual wControl *			addHelGroup(lua_State* config, unsigned char countryID) = 0;
	virtual wControl *			addColumn(lua_State* config, unsigned char countryID) = 0;
	virtual wControl *			addSquadron(lua_State* config, unsigned char countryID) = 0;
	virtual ObjectID			addStaticObject(lua_State * L, unsigned char countryID, bool dead = false) = 0;
    virtual ObjectID	        addStaticObject(unsigned char countryID, const StaticObjectData & data, bool dead = false) = 0;

	//Bullseye and reference point modification
	virtual void				setMainRefPoint(const cPoint & point) = 0;
	virtual void				addRefPoint(const RefPoint & worldPoint) = 0;

	//World event handling
	virtual void				onShoot(wcCoalitionName coalition, ObjectID shooterID, const wsType & weaponType, void * weaponData, ObjectID weaponID, ObjectID targetID, const cPoint & targetPoint, wModelTime impactTime = 0.0) = 0;
	virtual void				onDamage(wcCoalitionName coalition, ObjectID shooterID, ObjectID weaponID, ObjectID targetID, float deltaLife) = 0;
	virtual bool				detectMissile(const woPointer & pMissile, bool missileIsRadiating) = 0;

	//Sharing of contacts with targets
	virtual bool				updateInfoLevelAbout(wControl * pTargetGroup, bool positionKnown, int typeInfoLevel) = 0;

	//Bullseye and reference points accessors
	virtual const cPoint &		getMainRefPoint() const = 0;
	virtual const RefPoints &	getRefPoints() const = 0;
	virtual const CountryMembers &	getCountryMembers() const = 0;
	virtual const Players &		getPlayers() const = 0;

	//Service providers management
	virtual void				addServiceProvider(CountryService countryService, ObjectID objectID) = 0;
	virtual void				removeServiceProvider(CountryService countryService, ObjectID objectID) = 0;
	virtual const ServiceProviders & getServiceProviders(CountryService countryService) const = 0;
	virtual void				sendMessageToServiceProvider(wMessage * message, CountryService countryService, ObjectID objectID) = 0;

	//AWACS
	virtual void				addAWACSTarget(ObjectID id, const wcAWACS_target & target) = 0;
	virtual void				updateAWACSTarget(ObjectID id, const wcAWACS_target & target) = 0;
	virtual void				removeAWACSTarget(ObjectID id) = 0;
	virtual const AWACSTargetDataMap &	getAWACSTargets() const = 0;

	//Related to player unit switch
    virtual void                validatePointerOnObjectID(ObjectID ID_, woPointer woPnt) = 0;

	//Specific operations
	virtual void				finishTasks() = 0;
	virtual void				countryClear() = 0;
	virtual bool				Recover(long id_ = -1) = 0;

	//Coalition and state
	virtual bool				IsState(unsigned char state) = 0;
	virtual wcCoalitionName		Coalition() const = 0;

    virtual ICargoMan *         getCargoManager() = 0;
};
