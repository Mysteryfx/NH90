#pragma once

#include "SoundTools.h"
#include "wActivityManager.h"

class woCar;

class CrewSoundWrapper
{
public:
	CrewSoundWrapper(woCar* hostCar);
	~CrewSoundWrapper();

public:
	bool initSounder(ed::string sounderName, ed::string crewLocale);

	void stopSounderActivity();
	void startSounderActivity();

public:
	void sendEvent_GunnerEnemyHit();
	void sendEvent_GunnerEnemyDestroyed();
	void sendEvent_GunnerEnemyBurn();
	void sendEvent_GunnerEnemyHitWAP();
	void sendEvent_GunnerWeGettingDamage();

private:
	ed::Sounder sounder;
	woCar* host;

	typedef wActivity<CrewSoundWrapper> wActivityCrewSoundWrapper;
	wActivityManager crewSoundWrapperActivities;
	wtPointer wrapperActivity;

	void wrapperActivityHandler(wModelTime& modelTime);
};