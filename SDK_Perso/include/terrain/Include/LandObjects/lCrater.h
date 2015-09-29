#pragma once

#include "Terrain.h"
#include "LandObjects\lLandObject.h"
#include "wTime.h"
#include "Randomizer\RandomObject.h"

#define IID_lCrater Common::identify<lCrater>()

// Наземный объект - воронка
// Ставится со случайной ориентацией и исчезает через определенный промежуток времени
class TERRAIN_API lCrater : public lLandObject, public LinkHost
{
protected:
	// Деятельность, уничтожающая объект
	class Cleaner : public wTime
	{
	private:
		Link<lCrater> crater;
	public:
		Cleaner(lCrater *crater, wModelTime lifetime);
		virtual void NextEvent();
	};

public:
	static float Time_of_Repair; // Время починки аэродрома

	static RandomObject randomEvenly;
	static lLandObject* CreateObject();

	virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);
};
