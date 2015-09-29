#ifndef __Route__
#define __Route__

//Route

#include "WorldGeneral.h"
#include "cLinear.h"
#include "Registry/Registered.h"
#include <ed/vector.h>

namespace Lua
{
	class Config;
}
typedef double wModelTime;

namespace AI
{

//Waypoint of a route
struct WORLDGENERAL_API Waypoint
{
	Waypoint();
	void load(bool airborne, Lua::Config & config);
	//Waypoint types for aircraft route
	enum TypeAircraft
	{
		TYPE_TAKEOFF_FROM_PARKING = 0, //Takeoff from parking ramp
		TYPE_TAKEOFF_FROM_RUNWAY, //Takeoff from runway
		TYPE_WPT_OVR, //Round the turn
		TYPE_WPT_FBY, //Fly over the waypoint with the heading of the previous route leg
		TYPE_WPT_FIN, //Fly over the waypoint with the heading of the next route leg
		TYPE_LANDING //Land
	};
	//Waypoint types for ground route
	enum TypeVehicle
	{
		TYPE_ROAD = 0,
		TYPE_NOT_DEFINED,
		TYPE_COLUMN,
		TYPE_ROW,
		TYPE_WEDGE,
		TYPE_VEE,
		TYPE_DIAMOND,
		TYPE_ECHELON_RIGHT,
		TYPE_ECHELON_LEFT,
		TYPE_FREE
	};
	union {
		TypeAircraft	aircraft;
		TypeVehicle		vehicle;
	} type;
	cPoint			point; //Waypoint position including altitude
	enum AltType 	{ ALT_TYPE_BARO, ALT_TYPE_RADIO };
	AltType			altType; //Altitude type
	float			speed; //Desired speed at the way TO the waypoint
	bool			flagSpeed; //Velocity is locked
	wModelTime		time; //TOT or ETA value
	bool			flagETA; //TOT or ETA flag
	unsigned long	idRoute; //для ручного добавления
};

//Route
struct WORLDGENERAL_API Route
{
	Route();
	void	load(bool airborne, Lua::Config & config);
	Waypoint & operator [] (int index);
	bool	empty() const;
	size_t	size() const;
	typedef ed::vector<Waypoint> Waypoints;
	Waypoints		waypoints; //Waypoints
	bool			land; //Should the unit/group land after passing of the last waypoint
	ObjectID		destinationID; //Airbase to arrive (if land is true)
	bool			routeRelativeTOT; //Waypoint TOT is given relative to the route start time
private:
	bool	loadDestination_(Lua::Config & config);
};

}

#endif __Route__