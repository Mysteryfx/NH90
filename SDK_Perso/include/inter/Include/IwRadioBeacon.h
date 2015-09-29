#pragma once

#include "Inter.h"
#include "ed_string.h"

/*
Radio beacon have one or more transmitters.
Transmitters radiate specific signals and transmit specific messages (usually the beacon's callsign in morze code) to the broadcast.
The messages, parameters of the transmitters and the signals are determined by beacon's system. This information is stored 
in the beacon type database (./Scripts/World/Radio/BeaconSites.lua).
Beacon system is usually depended on beacon type, but there are may be exceptions.
The messages and the signals also determined by beacon's parameters (callsign, frequency). The information is in the
beacon database (Beacons.lua).
Beacons have systems: antennas, power supply, transmitters. Each system of a beacon can be connected to the body: unit or
land object. When the body is destroyed the system id disabled.
*/

enum wBeaconStatus
{
	BEACON_DISABLED = 0,	//Will not be added into the world
	BEACON_ACTIVE,			//Default status
	BEACON_INACTIVE,		//Body only
	BEACON_DESTROYED		//Destroyed body only
};

enum BeaconType
{
	BEACON_TYPE_VOID			 = 0x0000000,
	BEACON_TYPE_VOR			     = 0x0000001,
	BEACON_TYPE_DME              = 0x0000002,
	BEACON_TYPE_TACAN            = 0x0000004,
	BEACON_TYPE_HOMER            = 0x0000008,
	BEACON_TYPE_OUTER_MARKER	 = 0x0000010,
	BEACON_TYPE_MIDDLE_MARKER	 = 0x0000020,
	BEACON_TYPE_INNER_MARKER	 = 0x0000040,
	BEACON_TYPE_RSBN             = 0x0000080,
	BEACON_TYPE_LOCALIZER        = 0x0000100,
	BEACON_TYPE_GLIDESLOPE       = 0x0000200,
	//--------------------------------------
	BROADCAST_STATION			 = 0x0000400,
	//--------------------------------------
	AIRPORT_COMMUNICATION		 = 0x0001000,
	ROUTE_COMMUNICATION			 = 0x0002000,
	ILS_COMMUNICATION			 = 0x0004000,
	PRMG_COMMUNICATION			 = 0x0008000,
	NAUTICAL_COMMUNICATION		 = 0x0010000,
	//--------------------------------------
	BEACON_TYPE_MARKER						= BEACON_TYPE_OUTER_MARKER | BEACON_TYPE_MIDDLE_MARKER | BEACON_TYPE_INNER_MARKER,
	BEACON_TYPES							=	BEACON_TYPE_VOR | BEACON_TYPE_DME | BEACON_TYPE_TACAN |	BEACON_TYPE_HOMER |BEACON_TYPE_MARKER |
												BEACON_TYPE_RSBN | BEACON_TYPE_LOCALIZER | BEACON_TYPE_GLIDESLOPE | BROADCAST_STATION,
	
	//NAV
	BEACON_TYPE_VOR_DME						= BEACON_TYPE_VOR | BEACON_TYPE_DME,
	BEACON_TYPE_VORTAC						= BEACON_TYPE_VOR | BEACON_TYPE_TACAN,
	
	//AIRPORT_COMMUNICATION
	BEACON_TYPE_AIRPORT_HOMER				= AIRPORT_COMMUNICATION | BEACON_TYPE_HOMER, //ОПРС
	BEACON_TYPE_AIRPORT_HOMER_WITH_MARKER	= AIRPORT_COMMUNICATION | BEACON_TYPE_HOMER | BEACON_TYPE_MIDDLE_MARKER, //ОПРМ
	
	//ILS_COMMUNICATION (markers, ILS system)
	BEACON_TYPE_ILS_HOMER					= ILS_COMMUNICATION | BEACON_TYPE_HOMER,
	BEACON_TYPE_ILS_FAR_HOMER				= BEACON_TYPE_ILS_HOMER | BEACON_TYPE_OUTER_MARKER, //ДПРМ
	BEACON_TYPE_ILS_NEAR_HOMER				= BEACON_TYPE_ILS_HOMER | BEACON_TYPE_MIDDLE_MARKER, //БПРМ
	BEACON_TYPE_ILS_HOMER_WITH_MARKER		= BEACON_TYPE_ILS_FAR_HOMER | BEACON_TYPE_ILS_NEAR_HOMER, //ПРМ (ДПРМ, БПРМ)
	BEACON_TYPE_ILS_LOCALIZER				= ILS_COMMUNICATION | BEACON_TYPE_LOCALIZER,
	BEACON_TYPE_ILS_GLIDESLOPE				= ILS_COMMUNICATION | BEACON_TYPE_GLIDESLOPE,

	//PRMG (Russian millitary ILS) COMMUNICATION
	BEACON_TYPE_PRMG_LOCALIZER				= PRMG_COMMUNICATION | BEACON_TYPE_LOCALIZER,
	BEACON_TYPE_PRMG_GLIDESLOPE				= PRMG_COMMUNICATION | BEACON_TYPE_GLIDESLOPE,

	//NAUTICAL_COMMUNICATION
	BEACON_TYPE_NAUTICAL_HOMER				= NAUTICAL_COMMUNICATION | BEACON_TYPE_HOMER,
	
	//MORZE SIGNAL
	BECON_TYPE_MORZE_SIGNAL					=	BEACON_TYPE_VOR | BEACON_TYPE_TACAN | BEACON_TYPE_HOMER |
												BEACON_TYPE_MARKER | BEACON_TYPE_RSBN | BEACON_TYPE_LOCALIZER,
};

class cPosition;

#include "wTime.h"

//Radio beacon interface
class  INTER_API IwRadioBeacon 
{
public:
	virtual ~IwRadioBeacon() = 0 {}

	virtual void		  activate(bool flag_in, bool first = false) = 0; //Starts/stops signals/messages transmission
	virtual cPosition  &  Position(wModelTime t) = 0; //Returns position of the beacon body
	virtual const char  * get_name() const = 0;
	virtual const char  * get_callsign(bool morze = true) const = 0;
	virtual double		  get_frequency() const = 0;
	virtual bool		  activated() const = 0;	
	virtual BeaconType    get_type() const = 0;
	virtual int           get_system_id() const = 0; //Returns the beacon's system id
};

inline size_t makeBeaconTypeName(char * buff, size_t max, BeaconType beaconType)
{
	size_t index = 0;
	struct TypeName
	{
		BeaconType		type;
		const char*		name;
	};
	static TypeName typeNames[] = {	{ BEACON_TYPE_VOR,				"VOR"},
									{ BEACON_TYPE_DME,				"DME"},
									{ BEACON_TYPE_TACAN,			"TACAN"},
									{ BEACON_TYPE_HOMER,			"NDB"},
									{ BEACON_TYPE_OUTER_MARKER,		"OUTER MARKER" },
									{ BEACON_TYPE_MIDDLE_MARKER,	"MIDDLE MARKER" },
									{ BEACON_TYPE_INNER_MARKER,		"INNER MARKER" },
									{ BEACON_TYPE_MARKER,			"MARKER"},
									{ BROADCAST_STATION,			"BROADCAST"},
									{ BEACON_TYPE_LOCALIZER,		"LOCALIZER" },
									{ BEACON_TYPE_GLIDESLOPE,		"GLIDESLOPE" },
									{ AIRPORT_COMMUNICATION,		"AIRPORT"},
									{ ILS_COMMUNICATION,			"ILS"}	};
	bool first = true;
	for(unsigned int i = 0; i < sizeof(typeNames) / sizeof(TypeName); i++)
	{
		if(beaconType & typeNames[i].type)
		{
			if(!first)
			{
				index += ED_snprintf(buff + index, max - index, ", ");
			}
			index += ED_snprintf(buff + index, max - index, typeNames[i].name);
			first = false;
		}
	}
	return index;
}