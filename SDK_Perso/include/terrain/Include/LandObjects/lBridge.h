#pragma once

#include "Terrain.h"
#include "lStandartStructure.h"

#define IID_lBridge Common::identify<lBridge>()
#define IID_lTunnel Common::identify<lTunnel>()

class lRoadDestroyedObject;

// интерфейс оповещения
class TERRAIN_API BridgeListener
{
public:
	virtual void OnRoadObjectDestroyed(const lRoadDestroyedObject*bridge) = 0;
};

class TERRAIN_API lRoadDestroyedObject: public lStandartStructure
{
public:
	void Degrade(bool silent);
	bool IsDegradate() const;

protected:
	lRoadDestroyedObject();

protected:
	bool _isDegradate;
};

// Класс для мостов, они оповещают едущие по ним машинки о своем уничтожении
class TERRAIN_API lBridge : public lRoadDestroyedObject
{
public:
	static lLandObject* CreateObject();
};

// Класс для туннелей, они оповещают едущие по ним машинки о своем уничтожении
class TERRAIN_API lTunnel : public lRoadDestroyedObject
{
public:
	static lLandObject* CreateObject();
};