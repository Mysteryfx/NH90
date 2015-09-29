#pragma  once

#include "Formation.h"
#include <osg\Vec2d>
#include "TrailLink.h"

class FormationRow: public Formation
{
public:
	FormationRow();
	FormationRow(double offset)
		:_sideOff(offset)
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

	VehicleLink parent(int groupIndex) const;

	int newGroupIndex(int removedIndex, int oldIndex, int groupSize) const;

	CarFormationType type() const {return fRow;}

	bool operator==(const Formation&) const;
private:
	const double _sideOff;
};