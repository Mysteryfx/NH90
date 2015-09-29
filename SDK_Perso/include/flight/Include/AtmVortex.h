#pragma once
#include "Flight.h"
#include "AtmosphericEffect.h"

// воздушный вихрь
class FLIGHT_API AtmVortex : public AtmosphericEffect, public wTime
{
public:
	AtmVortex(const dVector& p1, const dVector& p2, float power, float radius, wModelTime lifetime);

	virtual void getAirState(const dVector& p, cVector &v);

	virtual IntersectionType TestVolume(ClipVolume *volume);
	virtual void serialize(Serializer &serializer);

	void NextEvent();

protected:
	dVector p1,p2, dp;
	float power, radius;
	wModelTime lifetime, t0;
};
