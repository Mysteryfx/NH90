#pragma once

#include "Vehicles\SimpleVehicle.h"
#include "CarPath.h"
#include <ed/map.h>

class PathHistory
{
public:
	PathHistory();
	PathHistory(const SimpleVehicle& host);
	~PathHistory();

	void setHost(const SimpleVehicle& host)
	{
		reset();
		_host = &host;
	};

	void reset();
	void clear();

	void update(double maxSpeed);

	Mem::Ptr<Trail> appendTrail(const double offset);
	Mem::Ptr<Trail> trail(const double offset) const;

	bool enabled() const {return !_trails.empty();}

	bool contains(const Trail* trail) const;
	bool empty() const {return _trails.empty();}

	typedef ed::map<double, Mem::Ptr<Trail> > Trails;
	const Trails& trails() const {return _trails;}

protected:
	void updateHistory(const LocalSpace2d& pos, double maxSpeed);

private:
	Trails _trails;
	const SimpleVehicle* _host;
};