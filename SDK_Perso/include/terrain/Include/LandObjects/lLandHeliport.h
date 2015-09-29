#pragma once

#include "Terrain.h"
#include "lObjWithLights.h"

#define IID_lLandHeliport Common::identify<lLandHeliport>()

/* рисуемая часть вертолётной площадки */
class TERRAIN_API lLandHeliport : public lObjWithLights
{
public:
	static lLandObject* lLandHeliport::CreateObject();

	virtual ~lLandHeliport();

	void        setHeliportID(ObjectID ID) { HeliportID = ID; }
	ObjectID	getHeliportID() const { return HeliportID; }

	virtual void Degrade(bool silent);

private:
	ObjectID		HeliportID; //ID woPointer'a, который привязывается к этому объекту
};
