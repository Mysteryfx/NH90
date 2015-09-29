#pragma once

#include "Terrain.h"

#include "LandObjects/lLandObject.h"

// два типа кранов:

#define IID_lTowerCrane Common::identify<lTowerCrane>()
#define IID_lPortCrane Common::identify<lPortCrane>()

// башенный
class TERRAIN_API lTowerCrane : public lLandObject
{
private:
	// константы скорости движений крана
	static const double timeRotate180;
	static const double timePolzunMove;
	static const double total_time;
	
	Graphics::DParamList drawArgs; 
	float phase; // случайная фаза - для разсинхронизации

	void RecalculateArgs(wModelTime t);
//	bool AllowDrawBlocks();

public:
	static lLandObject* CreateObject();
	lTowerCrane();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();
};

// портовый
class TERRAIN_API lPortCrane : public lLandObject
{
private:
	// константы скорости движений крана
	static const double timeRotate180;
	static const double timeStrelaDown;
	static const double total_time;
	
	Graphics::DParamList drawArgs; 
	float phase; // случайная фаза - для разсинхронизации

	void RecalculateArgs(wModelTime t);
//	bool AllowDrawBlocks();

public:
	static lLandObject* CreateObject();
	lPortCrane();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();
};

