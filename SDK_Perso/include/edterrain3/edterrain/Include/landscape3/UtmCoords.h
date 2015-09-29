#ifndef __LANDSCAPE3_UTMCOORDS_H
#define __LANDSCAPE3_UTMCOORDS_H
#include "../landscape3_config.h"

struct lua_State;

// functions for the UTM (Universal Transverse Mercator) projection
// based on the data from http://www.uwgb.edu/dutchs/UsefulData/UTMFormulas.htm
namespace utm_private
{

// projection definition: datum, UTM strip & offset
struct projection_t
{
	double a;           // equatorial radius, meters
	double b;           // polar radius, meters
	double long0;       // central meridian of zone, degrees
	double k0;          // scale along long0
	double feasting;    // false easting, to be added to x
	double fnorthing;   // false northing, to be added to y

	// dcs origin UTM coordinates
	double dcs_origin_x, dcs_origin_y;

	int utmzone;

	struct GeographicProjection
	{
		double originLat;
		double originLon;
		double meterPerDegreeLat;
		double meterPerDegreeLon;
	};
	GeographicProjection geographicProjection;

	enum PROJECTIONTYPE
	{
		PT_UTM,
		PT_GEOGRAPHIC,
	};
	PROJECTIONTYPE projectionType;

	EDTERRAIN_API projection_t();
};

// convert x,y in UTM coords, to lat,lon (in degrees, N+, E+)
EDTERRAIN_API void toLatLon(const projection_t& proj, double lockon_z, double lockon_x, double& lat, double& lon);

// convert lat,lon (degrees, N+, E+) to UTM coords
EDTERRAIN_API void fromLatLon(const projection_t& proj, double lat, double lon, double& lockon_z, double& lockon_x);

// convert lockon to MGRS
EDTERRAIN_API ed::string fromLockonToMGRS(const projection_t& proj, double lockon_z, double lockon_x, int prec = 5);

// convert from UTM to lockon
EDTERRAIN_API void fromUTMtoLockon(const projection_t& proj, double utm_x, double utm_y, double& lockon_x, double& lockon_z);

// convert from lockon to UTM
EDTERRAIN_API void fromLockontoUTM(const projection_t& proj, double lockon_x, double lockon_z, double& utm_x, double& utm_y);

// load projection definition from lua file
EDTERRAIN_API bool ParseConfig(const char* cfgfilename, projection_t& projection);
EDTERRAIN_API bool ParseConfig(lua_State* LS, projection_t& projection);

}

#endif // UTM_COORDS_H
