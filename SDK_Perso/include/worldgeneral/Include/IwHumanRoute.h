#pragma once 

// Laser Warning System interface

#include "WorldGeneral.h"

struct Waypoint;
class WORLDGENERAL_API IwHumanRoute
{
public:
	virtual ~IwHumanRoute() = 0;

	virtual bool getEndOfRoute_flag() = 0;
	virtual int  getCurWayPnt() = 0;

	virtual void setEndOfRoute_flag(bool) = 0;
	virtual void setCurWayPnt(int) = 0;
    virtual Waypoint * getWayPnts() = 0;
    virtual int  getWayPntsCount() const = 0;

};
