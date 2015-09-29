#pragma  once

#include <osg\Vec2d>
#include "TrailLink.h"
#include "Mem\Shared.h"
#include "CarsEnums.h"

class Formation: public Mem::Shared
{
public:
	virtual osg::Vec2d offset(int groupIndex) const = 0;
	//относительный отступ от вирутального лидера

	virtual double length(int size) const = 0;
	virtual double width(int size) const = 0;
	virtual double widthR(int size) const = 0;
	virtual double widthL(int size) const = 0;

	virtual double aheadLeaderDist(int size) const = 0;
	virtual double behindLeaderDist(int size) const = 0;

	virtual double turnR(int size) const = 0;

	virtual VehicleLink parent(int groupIndex) const = 0;

	virtual int newGroupIndex(int removedIndex, int oldIndex, int groupSize) const = 0;

	virtual CarFormationType type() const = 0;

	virtual bool operator==(const Formation&) const = 0;
	bool operator !=(const Formation& rhs) const {return !(*this == rhs);};
};