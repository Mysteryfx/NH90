#pragma once


#include "Terrain.h"

#include "LandObjects/lLandObject.h"


// радары на аэродроме

#define IID_lRadarQuery Common::identify<lRadarQuery>()
#define IID_lRadarRLS37 Common::identify<lRadarRLS37>()
#define IID_lRadarRSP7 Common::identify<lRadarRSP7>()

// Радар, просто крутящийся
class TERRAIN_API lSimpleRadar : public lLandObject
{
protected:
	Graphics::DParamList drawArgs;
	double speed;
	
	virtual void RecalculateArgs(wModelTime t);

//	bool AllowDrawBlocks();

public:
	// скорость - секунд на оборот
	lSimpleRadar(double _speed);

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();
};

// Радар запроса
class TERRAIN_API lRadarQuery : public lSimpleRadar
{
public:
	static lLandObject* CreateObject();
	lRadarQuery() : lSimpleRadar(2) {};
};

// РЛС-37
class TERRAIN_API lRadarRLS37 : public lSimpleRadar
{
public:
	static lLandObject* CreateObject();
	lRadarRLS37() : lSimpleRadar(4) {};
};

// РСП-7 (вертится туда-сюда в двух плоскостях)
class TERRAIN_API lRadarRSP7 : public lSimpleRadar
{
protected:
	virtual float CalculateArg(double turn);
	virtual void RecalculateArgs(wModelTime t);

public:
	static lLandObject* CreateObject();
	lRadarRSP7();
};

