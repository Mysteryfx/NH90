#pragma once

#include "Terrain.h"
#include "LandObjects/lLandObject.h"

#define IID_lLandVehicle Common::identify<lLandVehicle>()

// Машинка, как статический объект
class TERRAIN_API lLandVehicle : public lLandObject
{
public:
	static lLandObject* CreateObject();

	lLandVehicle();

	virtual void Degrade(bool silent);
    virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);
	virtual const Graphics::DParamList& GetDrawArguments() { return drawArgs; }

private:
	Graphics::DParamList drawArgs;
};
