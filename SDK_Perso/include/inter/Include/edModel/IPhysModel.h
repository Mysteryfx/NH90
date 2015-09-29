#ifndef ED_MODEL_IPHYSMODEL_H
#define ED_MODEL_IPHYSMODEL_H

#include "ModelLoadState.h"
#include "ModelParams.h"
#include "IModel.h"

// bullet
class btCollisionShape;
class btTransform;

// osg
namespace osg{
	class Matrixd;
};

namespace model
{

class IModel;

/// Common interface for graphics models.
class IPhysModel
{
public:
	virtual ~IPhysModel() {}

	/// Returns base model interface.
	virtual const IModel* base()const = 0;
	/// Returns base model interface.
	virtual IModel* base() = 0;

	/// Means than model will be used in current frame. So it must start loading.
	virtual void needIt()=0;	

	/// This method forces model to load immediately.
	virtual void forceLoad()=0;

	/// Returns current state of model loading.
	virtual LoadState getLoadState()const = 0;

	/// Returns array of all collision shapes of model.
	/// Size of array you can get from IModel::getNumCollisionShells().
	virtual btCollisionShape** getCollisionShapes() = 0;

	/// Returns array of opsitions of all collision shells of model.
	/// Size of array you can get from IModel::getNumCollisionShells().
	virtual btTransform* getCollisionShapesPos(const osg::Matrixd &modelPos, const ModelParams &params = model::dummyModelParams) = 0;
};

}
#endif
