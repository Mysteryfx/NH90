#pragma once

#include "Terrain.h"
#include "wTime.h"
#include "LandObjects\lObjectDynData.h"

#define IID_lForest Common::identify<lForest>()

// лесной массив (деревья)
class TERRAIN_API lForest : public lLandObject
{
	
	struct lForestDynData : public lObjectDynData
	{
		// Копия граф. объекта
		gShapeObject* pgcopy;

		lForestDynData(lLandObject *owner);
	};

public:
	static lLandObject* CreateObject();
	lForest();
	virtual ~lForest();

	// вызывается для столкновений
	virtual void PostCollisionProcessing(wsCollisionData *info, ObjectID obj);

//	virtual gShapeObject *Shape(wModelTime t);
	virtual void Degrade(bool silent);
};

