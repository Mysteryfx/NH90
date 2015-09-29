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
	// �������
	virtual void release(){};

public:
	// ������� SceneObject �� �������� � �������
	virtual void removeSceneObject(edterrain::SceneObjectId id);
		
private:
	ITerrainImpl3* mTerrain3;
};


