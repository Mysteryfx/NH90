#pragma once

#include "Terrain.h"
#include "LandObjects/lLandObject.h"

#define IID_lLandFort Common::identify<lLandFort>()

// Бункеры и другие статические объекты
class TERRAIN_API lLandFort : public lLandObject
{
public:
	static lLandObject* CreateObject();

	virtual void Degrade(bool silent);
};
