#pragma once

#include "Terrain.h"
#include "wTime.h"

#include "LandObjects/lLandObject.h"

#define IID_lCrossing Common::identify<lCrossing>()

enum CrossingAction
{
	caOpened,
	caOpening,
	caClosing,
	caClosed
};

class lCrossing;

// деятельность: открытие/закрытие переезда (создается только на время открытия/закрытия)
class TERRAIN_API lCrossingActivity : public wTime
{
private:
	lCrossing *me;

public:
	lCrossingActivity(lCrossing *_me);
	
	virtual void NextEvent();
};

// железнодорожный переезд
class TERRAIN_API lCrossing : public lLandObject
{
friend class lCrossingActivity;
private:
	static const double OpenTime;

	Graphics::DParamList drawArgs;

	wModelTime actionStart;
	CrossingAction action;

	Link<lCrossingActivity> activity;
	
public:
	static lLandObject* CreateObject();
	lCrossing();
	virtual ~lCrossing();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments();

	// управление переездом
	void Close();
	void Open();
	bool IsOpen();
	
//	bool AllowDrawBlocks();
};
