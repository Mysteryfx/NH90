#pragma once
#include <osg\Vec2d>
#include "Tools.h"

class MovingVehicle;
class TrailVehicle;
class Trail;
enum VehicleStateType;

class FormationState
{
public:
	FormationState(MovingVehicle *host)
		:_host(host){}

	virtual void determineForce(osg::Vec2d &steering, double &breaking) = 0;

	virtual void update(double /*elapsedTime*/) = 0;

	//Позиция, куда мы стремимся в текущий момент, при езде по пути учитываются предшественники
	virtual osg::Vec2d arrivedPos() const {return osg::zeroVD;}
	virtual double arrivedDist() const;

	//Идеальная позиция относительно виртуального лидера (используется при остановке в конце пути)
	virtual void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const {pos = osg::zeroVD; dir = osg::zeroVD;}

	virtual double desiredSpeed() const = 0;
	virtual double pathParam() const = 0;

	virtual VehicleStateType type() const = 0;

protected:
	MovingVehicle *_host;
};
