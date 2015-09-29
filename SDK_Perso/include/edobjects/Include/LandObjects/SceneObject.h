#pragma 
#include "viObjectHandle.h"
//temp
#include "edTerrain/ITerrain.h"

class SceneObject : public ISceneObject
{
public:
	explicit SceneObject(edterrain::SceneObjectId id);

	//Scene ObjectInterface
	unsigned int               getObjectID()      const override;
	osg::Matrixd               getObjectPosition()const override;
	model::IModel*             getObjectType()    const override;
	const model::IModelParams* getModelParams()   const override;
private:
	edterrain::SceneObjectId mObjectId;
};