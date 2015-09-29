//                    file wAirService.h
#pragma once

#include "Transport.h"
#include "wActivityManager.h"
#include "Common.h"
#include <ed/vector.h>
#include "wMovingObject.h"

class woShip;
class woATC;
class wAirService;

#define IID_IAirService Common::identify<wAirService>() 

class TRANSPORT_API wAirService :	public Common::FakeUnknown<Serializable>
{
public:

	wAirService(woShip* host_ = 0);
	virtual ~wAirService();

	void				setHost(woShip* host_, int numParking);
	bool				createSystem(int callsign_, float frequency);
	bool				deleteSystem();
	woATC*				getATC()			{return pATC;}
	void				controlService(wModelTime& timeStep);
	bool				deckFreeFromPlanes();
    void				serialize(Serializer &);
	void				SetParking(int num, woPointer pPlane);
protected:

	typedef				ed::vector<woPointer> TakeOffList;
	typedef				wActivity<wAirService> wActivityAirService;
	wActivityManager	activities;

	woShip*				host;
	woATC*				pATC;
	TakeOffList			takeOffList;
};
