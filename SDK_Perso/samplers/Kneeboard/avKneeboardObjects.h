#pragma once
#include "Avionics/avMovingMapPoint.h"
namespace Lua
{
	class Config;
}

namespace cockpit
{

class COCKPITBASE_API  avKneeboardZoneObject :  public SimpleOctTreeLoad , 
												public avDrawCashedObj
{
public:
	avKneeboardZoneObject(void);
	avKneeboardZoneObject(Lua::Config & config,float lat_lon_aspect); //read constructor

	~avKneeboardZoneObject(void){};

	void  set_points(const ed::vector<Vector3> & pnts);

	const ed::vector<Vector3> & get_points() const { return points;}
protected:
	ed::vector<Vector3>    points;
};

}