#pragma  once

#include "Formation.h"
#include <osg\Vec2d>
#include "TrailLink.h"

class FormationDiamond: public Formation
{
public:
	FormationDiamond();
	FormationDiamond(double forwardOffset, double sideOffset)
		:_fwdOff(-forwardOffset), _sideOff(sideOffset)
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

	CarFormationType type() const {return fDiamond;}

	bool operator==(const Formation&) const;

private:
	int carInColumn(int carIndex) const {return carIndex/3;}
	double columnDistOffset(int groupIndex) const;
	int columnIndex(int groupIndex) const;

	const double _fwdOff;
	const double _sideOff;

};