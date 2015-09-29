#ifndef ED_MODEL_IMODEL_H
#define ED_MODEL_IMODEL_H

#include "ModelParams.h"
#include "ModelLoadState.h"
#include "osg/BoundingBox"
#include "ed/string.h"

#include "osg/OSGMath.h"

#include <stdint.h>

namespace offshore{ struct Scene;};

namespace offshore
{
	struct Mesh;
}

namespace model
{

class IConnector;
class ICollisionShell;
class ILight;

/// Common interface for models.
class IModel
{
protected:
	IModel(){}
public:
	virtual ~IModel() {}

	/// Returns the number of transform nodes in model.
	virtual unsigned int getNumTransformNodes()const=0;	

	/// Returns current state of model loading.
	virtual LoadState getLoadState()const = 0;

	/// Returns model name, which is also its hash key.
	/// Can't return NULL.
	virtual const char* getName()const=0;

	/// Returns bounding box of model.
	virtual const osg::BoundingBox& getBoundingBox()const=0;

	/// Returns bounding box of model. box type = BT_MAIN/BT_LIGHTING
	virtual osg::BoundingBox getBoundingBox(int boxtype)const{return osg::BoundingBox();};

	/// Returns light box of model. If model doesn't have light sources returns invalid box.
	virtual const osg::BoundingBox& getLightingBox()const=0;

	/// Returns number of model arguments.
	virtual unsigned int getNumArguments()const=0;

	/// Returns the number of connectors in model.
	virtual unsigned int getNumConnectors()const=0;

	/// Returns connector by its index.  If there is no if index > getNumConnectors() returns NULL.
	virtual const IConnector* getConnector(unsigned int index)const=0;

	/// Returns connector by its name. If there is no connector with such name returns NULL.
	virtual const IConnector* getConnectorByName(const char* name)const=0;

	/// Returns the number of collision nodes of model.
	/// Note: it doesn't take into account segment nodes.
	virtual unsigned int getNumCollisionShells()const=0;
	/// Returns shell node by index given by \param i.
	/// \param i must be less than the shell nodes number.
	/// Note: it doesn't take into account segment nodes.
	virtual const ICollisionShell* getCollisionShell(unsigned int i)const=0;

	/// Returns if model has light sources.
	inline bool hasLights()const{return getNumLights() != 0;}

	/// Returns the number of lights in model.
	virtual unsigned int getNumLights()const=0;

	/// Returns light by its index.
	/// \param index must be less then getNumLights().
	virtual const ILight* getLight(unsigned int index)const=0;

	/// Returns the number of bones in model.
	virtual unsigned int getNumBones()const=0;

	/// Returns id of bone or model::INVALID_BONE_ID if there is no bone with given name.
	virtual unsigned int getBoneIdByName(const char *name)const=0;
	
	/// Returns name of bone node or NULL if given id is invalid.
	virtual const char* getBoneName(unsigned int id)const=0;
	
	/// Returns matrix of bone or zero matrix if given id is invalid.
	virtual const osg::Matrixd& getBoneMatrix(unsigned int id)const=0;

	/// Means than model will be used in current frame. So it must start loading.
	virtual void needIt()=0;	

	/// This method forces model to load immediately.
	virtual void forceLoad()=0;

	/// Tests intersection with segment.
	/// \param t returns parametric position of intersection point.
	/// \param n returns normal in intersection point.
	/// Returns name of intersected collision shell or NULL if there was no intersection with any collision shell of model.
	virtual const char* testWithSegment(const osg::Vec3d &a, const osg::Vec3d &b, double &t, osg::Vec3d &n, const osg::Matrixd &modelPos, const IModelParams *params = nullptr) = 0;

	/// Tests intersection with segment.
	/// \param p returns position of intersection point.
	/// \param n returns normal in intersection point.
	/// Returns name of intersected collision shell or NULL if there was no intersection with any collision shell of model.
	inline const char* testWithSegment(const osg::Vec3d &a, const osg::Vec3d &b, osg::Vec3d &p, osg::Vec3d &n, const osg::Matrixd &modelPos, const IModelParams *params = nullptr){
		double tm = 2.0;
		auto res = testWithSegment(a, b, tm, n, modelPos, params);
		if(res == nullptr){
			return res;
		}
		p = ed::lerp(a, b, tm);

		return res;
	}

	/// Tests intersection with segment.
	/// \param p returns position of intersection point.
	/// \param n returns normal in intersection point.
	/// Returns name of intersected collision shell or NULL if there was no intersection with any collision shell of model.
	inline const char* testWithSegment(const osg::Vec3d &a, const osg::Vec3d &b, osg::Vec3d &p, double &t, osg::Vec3d &n, const osg::Matrixd &modelPos, const IModelParams *params = nullptr){
		auto res = testWithSegment(a, b, t, n, modelPos, params);
		if(res == nullptr){
			return res;
		}
		p = ed::lerp(a, b, t);

		return res;
	}


	/// Tests segments from one model with shell of target mode.
	/// \param p returns position of intersection point.
	/// \param n returns normal in intersection point.
	/// Returns if there is intersection.
	virtual bool testWithModel(const osg::Matrixd &modelPos, const IModelParams *params, IModel &target, const osg::Matrixd &targetPos, const IModelParams *targetParams, ed::string &segmentName, ed::string &shellName, osg::Vec3d &p, osg::Vec3d &n){return false;}

	/// export geometry of model to offshore scene
	virtual void exportToOffshore(offshore::Scene& scene, int rootnode, const osg::Matrixd &modelPos, const IModelParams *params = nullptr) const{}

	// Модель может инстансироваться
	virtual bool isCanBeInstanced()const{return false;}

	// Максимальная дистанция видимости. Для оптимального построения стореджей
	// modes - для какого режима
	virtual float getMaxViewDistance(uint32_t modes=(~0L))const{return 0;};

	// Маска семантик, для оптимизации запросов
	virtual uint32_t getSemanticMask()const{return 0;};

	// имя модели разрушения
	virtual const char* getCrashModelName() const {return "";}

	// прокси меш
	virtual const offshore::Mesh* getProxyMesh() const {return nullptr;};
};

static const model::IModel* INVALID_IModel = (model::IModel*)~0;

}

#endif
