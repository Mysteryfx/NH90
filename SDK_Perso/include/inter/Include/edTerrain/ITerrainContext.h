#ifndef __ITERRAINCONTEXT_H__
#define __ITERRAINCONTEXT_H__

#include "ITerrain.h"

namespace edterrain
{

class ITerrainContext
{

public:
	// serialize (параметры?)
	virtual void serialize(){};
	// удалить
	virtual void release(){};

public:
	// удалить SceneObject из запросов и рендера
	virtual void removeSceneObject(SceneObjectId id){}
};

}

#endif //__ITERRAIN_H__