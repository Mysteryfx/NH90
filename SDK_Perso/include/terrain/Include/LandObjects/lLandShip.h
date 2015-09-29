#pragma once

#include "Terrain.h"
#include "wTime.h"
#include "LandObjects/lLandObject.h"

class lLandShip;

#define IID_lLandShip Common::identify<lLandShip>()

// период между тиками затопления
const float SinkActivityPeriod = 0.01f; 

// деятельность: затопление корабля
class TERRAIN_API lSinkActivity : public wTime
{
friend class lLandShip;
private:
	lLandShip *me;

	// данные для затопления
	float Ship_Length;
	float Ship_Height;
	float Immersion_Time;
	
public:
	lSinkActivity(lLandShip *_me, wModelTime start);
	
	virtual void NextEvent();
};


// Кораблик, как статический объект
class TERRAIN_API lLandShip : public lLandObject
{
friend class lSinkActivity;
private:
	static Graphics::DParamList globalArgs;

	Link<lSinkActivity> sinking;

    // current positions with waves
    Position3 _curpos3;
    cPosition _curpos;

	// подготовка к затоплению корабля
	void ShipStartSink(wModelTime t);

	// затопление корабля (вызывается из деятельности)
	// возвращает true если окончательно затонул
	bool ShipSink(float time_step);

public:
	static lLandObject* CreateObject();
	lLandShip();
	~lLandShip();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments() {return globalArgs;}

	// вызывается для повреждений (должна создать динамическую информацию)
	virtual void TakeDamage(double amount, ObjectID hitman);

	// получить позиции с учетом волнения
	virtual const Position3& GetPosition();
	virtual cPosition &Position(wModelTime t);
};

