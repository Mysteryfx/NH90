#pragma  once

#include <ed/list.h>
#include "CarPath.h"
#include <osg\Vec2d>

class TrailVehicle;
class MovingVehicle;

class VehicleLink
{
public:
	
	enum Type
	{
		VehicleMovingLink,
		VehicleTrailLink
	};

	struct Forerunner
	{
		Forerunner(int pIndex, double pDist)
			:_linkGroupIndex(pIndex), _dist(pDist), _link(0){}

		void setVehicle(const MovingVehicle* veh);

		const MovingVehicle* vehicle() const {return _link;}
		int groupIndex() const {return _linkGroupIndex;}
		double dist() const {return _dist;}
	
	private:
		int _linkGroupIndex;
		double _dist;
		const MovingVehicle *_link;
	};

	typedef ed::list<Forerunner> Forerunners;
	
	static VehicleLink createVehicleLink(int groupIndex, const osg::Vec2d& offset);
	static VehicleLink createVehicleTrailLink(int groupIndex, 
		double trailDist, const double trailOffset, const Forerunners& previous);

	void setVehicle(const TrailVehicle *veh);
	void setTrail(Mem::Ptr<Trail> tr);

	virtual Type type() const {return _type;}

	int groupIndex() const {return _linkGroupIndex;}

	const TrailVehicle *vehicle() const {return _link;}

	const Trail* trail() const {return _trail.get();}

	double trailDist() const {return _trailDist;}
	void setTrailDist(double dist) {_trailDist = dist;}

	double trailOffset() const {return _offset.x();}
	const Forerunners& forerunners() const {return _forerunners;}
	Forerunners& forerunners() {return _forerunners;}

	const osg::Vec2d& offset() const {return _offset;}

private:
	VehicleLink(int pGroupIndex, const osg::Vec2d& offset);
	VehicleLink(int pGroupIndex, double pTrailDist, const double pTrailOffset, const Forerunners& pPrevious);
	
	double _trailDist;
	osg::Vec2d _offset;
	Forerunners _forerunners;
	const TrailVehicle *_link;
	int _linkGroupIndex;
	Type _type;

	Mem::Ptr<Trail> _trail;
};