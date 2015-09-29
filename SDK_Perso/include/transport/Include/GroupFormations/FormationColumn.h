#pragma  once

#include "Formation.h"
#include <osg\Vec2d>
#include "TrailLink.h"

class FormationColumn: public Formation
{
public:
	FormationColumn();
	FormationColumn(double offset)
		:_off(-offset)
	{
	}

	osg::Vec2d offset(int groupIndex) const;

	double length(int size) const;
	double width(int size) const;
	double widthR(int size) const;
	double widthL(int size) const;
	
	double turnR(int size) const;

	double aheadLeaderDist(int size) const;
	double behindLeaderDist(int size) const;

	virtual VehicleLink parent(int groupIndex) const;

	virtual int newGroupIndex(int removedIndex, int oldIndex, int groupSize) const;

	virtual CarFormationType type() const {return fColumn;}

	bool operator==(const Formation&) const;

protected:
	double columnDistOffset(int groupIndex) const;
	const double _off;
};