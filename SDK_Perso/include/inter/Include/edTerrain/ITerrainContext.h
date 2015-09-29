#ifndef __ITERRAINCONTEXT_H__
#define __ITERRAINCONTEXT_H__

#include "ITerrain.h"

namespace edterrain
{

class ITerrainContext
{

public:
	// serialize (���������?)
	virtual void serialize(){};
	// �������
	virtual void release(){};

public:
	// ������� SceneObject �� �������� � �������
	virtual void removeSceneObject(SceneObjectId id){}
};

}

#endif //__ITERRAIN_H__