#pragma  once

#include "Formation.h"
#include <osg\Vec2d>
#include "TrailLink.h"

class FormationVee: public Formation
{
public:
	FormationVee();
	FormationVee(double forwardOffset, double sideOffset, double startSideOffset)
		:_fwdOff(forwardOffset), _sideOff(sideOffset), _startSideOffset(startSideOffset)
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

	CarFormationType type() const {return fVee;}

	bool operator==(const Formation&) const;

private:
	int columnIndex(int groupNumber) const;
	int rowIndex(int groupNumber) const;

	const double _fwdOff;
	const double _sideOff;
	const double _startSideOffset;
};