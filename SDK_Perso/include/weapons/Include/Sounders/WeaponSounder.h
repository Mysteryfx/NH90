#pragma once

#include "SoundTools.h"
#include "woMovingVehicle.h"
#include "wWeaponUtils.h"

class WeaponSounder
{
public:
	WeaponSounder();
	~WeaponSounder();

public:
	bool initSounder(woPointer _host, ed::string _unitName);

	void update(float rotationSpeedY, int wsId);

	void sendTurretSoundCreate(wsystem_type sWsType, int wsId);
	void stopPlay(int WsId);
	bool isCreated() { return sounder.isCreated(); }

private:
	ed::Sounder sounder;
	woPointer host;
};