#pragma once

#include "Terrain.h"
#include "lStandartStructure.h"
#include "lSiteElement.h"

#define IID_lLighthouse Common::identify<lLighthouse>()
#define IID_lHomerBuilding	Common::identify<lHomerBuilding>()

// маяк
class TERRAIN_API lLighthouse : public lStandartStructure,
								public lSiteElement
{
protected:
	static Graphics::DParamList dayArgs;
	virtual void Degrade(bool silent);
	virtual bool is_light_on() const;
public:
	static lLandObject* CreateObject();
	lLighthouse();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();

	// блоки отрисовки
//	bool AllowDrawBlocks();
};

class TERRAIN_API lHomerBuilding : public lLighthouse
{
public:
	lHomerBuilding() {;}
	static lLandObject* CreateObject();
protected:
	virtual bool is_light_on() const;
};