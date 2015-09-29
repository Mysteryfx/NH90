#pragma  once

#include <osg\Vec2d>
#include <ed/vector.h>
#include "Formation.h"
#include "TrailLink.h"

class FormationFree: public Formation
{
public:
	FormationFree(const ed::vector<osg::Vec2d> &offsets);

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

	CarFormationType type() const {return fFree;}

	bool operator==(const Formation&) const;

	void onRemoveVehicle(int removedIndex);

private:
	void updateProperties();

	ed::vector<osg::Vec2d> _offsets;
	double _widthR, _widthL;
	double _aheadDist, _behindDist;
};