#pragma once

#include "renderer/IRenderAPI.h"
#include "Terrain.h"
#include "LandObjects/lLandObject.h"

#define IID_lDanceFloor Common::identify<lDanceFloor>()

// танцплощадка
class TERRAIN_API lDanceFloor : public lLandObject
{
private:
	// периоды 
	static const double timeRotate180;
	static const double total_time;

	Graphics::DParamList drawArgs; 
	float phase; // случайная фаза - для разсинхронизации

	void RecalculateArgs(wModelTime t);
//	bool AllowDrawBlocks();

public:
	static lLandObject* CreateObject();
	lDanceFloor();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();
};
