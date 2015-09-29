#pragma  once

#include "Formation.h"
#include <osg\Vec2d>
#include "TrailLink.h"

void getDefaultEchelonOff(double &fwd, double &side);

template <bool left, CarFormationType fType>
class FormationEchelon: public Formation
{
public:
	FormationEchelon()
	{
		getDefaultEchelonOff(_fwdOff, _sideOff);
		_sideOff = left ? -_sideOff : _sideOff;
	}

	FormationEchelon(double forwardOffset, double sideOffset)
		:_fwdOff(-forwardOffset), _sideOff(left ? -sideOffset : sideOffset)
	{
	}

	osg::Vec2d offset(int groupIndex) const
	{
		return osg::Vec2d(groupIndex*_sideOff, groupIndex*_fwdOff);
	}

	double length(int size) const
	{
		if (size > 0)
			return std::abs((size-1)*_fwdOff);
		return 0;
	}

	double width(int size) const
	{
		if (size > 0)
			return std::abs((size-1)*_sideOff);
		return 0;
	}

	double turnR(int size) const
	{
		return width(size);
	}


	double widthR(int size) const
	{
		if (size < 2)
			return 0;

		return _sideOff > 0. ? _sideOff*(size-1) : 0.;
	}

	double widthL(int size) const
	{
		if (size < 2)
			return 0;

		return _sideOff > 0. ? 0 : -_sideOff*(size-1);
	}

	double aheadLeaderDist(int size) const
	{
		return 0.;
	}

	double behindLeaderDist(int size) const
	{
		return length(size);
	}

	VehicleLink parent(int groupIndex) const
	{
		VehicleLink::Forerunners forerunners;
		if (groupIndex==0)
			return VehicleLink::createVehicleTrailLink(-1, 0., 0., forerunners);

		return VehicleLink::createVehicleTrailLink(-1, -groupIndex*_fwdOff, groupIndex*_sideOff, forerunners);
	}

	int newGroupIndex(int removedIndex, int oldIndex, int /*groupSize*/) const
	{
		return (oldIndex > removedIndex) ? oldIndex-1 : oldIndex;
	}

	CarFormationType type() const {return fType;}
	
	bool operator==(const Formation& rhs) const
	{
		if (rhs.type() != type())
			return false;

		const FormationEchelon& fRhs = static_cast<const FormationEchelon&>(rhs);
		return fRhs._fwdOff == _fwdOff && fRhs._sideOff == _sideOff;
	}

private:
	double _fwdOff;
	double _sideOff;
};

typedef FormationEchelon<true, fEchelonL> FormationEchelonL;
typedef FormationEchelon<false, fEchelonR> FormationEchelonR;