#pragma once

#include "SoundTools.h"
#include "woMovingVehicle.h"
#include "wWeaponUtils.h"

class TurretRotationSounder
{
public:
	TurretRotationSounder();
	~TurretRotationSounder();

public:
	bool initSounder(woPointer _host, ed::string _unitName, wsystem_type sWsType);
	void update(float rotationSpeedY);
	void stopPlay();
	bool isCreated() { return sounder.isCreated(); }

private:
	ed::Sounder sounder;
	woPointer host;
};