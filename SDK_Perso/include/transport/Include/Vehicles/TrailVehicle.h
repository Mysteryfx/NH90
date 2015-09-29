#pragma once
#include "PathHistory.h"
#include "SimpleVehicle.h"

class TrailVehicle: public SimpleVehicle
{
public:
	TrailVehicle();
	virtual ~TrailVehicle() {}

	void update(double maxSpeed);

	const Mem::Ptr<Trail> appendTrail(const double trailOffset) {return _trail.appendTrail(trailOffset);}
	bool isMyTrail(const Trail* tr) const;
	const Mem::Ptr<Trail> trail(const double trailOffset) const {return _trail.trail(trailOffset);}
	const PathHistory& trails() const {return _trail;}

	void removeTrails();

protected:
	PathHistory _trail;
};