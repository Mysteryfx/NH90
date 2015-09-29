#pragma once
#include "edTerrain.h"
#include "edterrain/ITerrainContext.h"

class ITerrainImpl3;

class EDTERRAIN_API ITerrainContextImpl3 : public edterrain::ITerrainContext
{
public:
	ITerrainContextImpl3(ITerrainImpl3* pterrain) :
		mTerrain3(pterrain){}

	// serialize
	virtual void serialize(){};
	// удалить
	virtual void release(){};

public:
	// удалить SceneObject из запросов и рендера
	virtual void removeSceneObject(edterrain::SceneObjectId id);
		
private:
	ITerrainImpl3* mTerrain3;
};


