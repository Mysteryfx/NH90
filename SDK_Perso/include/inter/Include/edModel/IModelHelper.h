#ifndef ED_MODEL_IMODEL_HELPER_H
#define ED_MODEL_IMODEL_HELPER_H

#include "edModel/IModel.h"

namespace model
{

template<typename T1, typename T2> class IModelHelper : public IModel
{
private:
	const T1 &obj;
public:
	IModelHelper(const T1 &obj):obj(obj){}

	virtual unsigned int getNumTransformNodes()const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getNumControlNodes() : 0;
	}	

	virtual const osg::BoundingBox& getBoundingBox()const{
		const static osg::BoundingBox dummy;

		const T2 *r = obj.getRootNode<false>();
		return (r != NULL)?r->getBBox():dummy;
	}

	virtual const osg::BoundingBox& getLightingBox()const{
		const static osg::BoundingBox dummy;

		const T2 *r = obj.getRootNode<false>();
		return (r != NULL)?r->getLightBox():dummy;
	}

	virtual unsigned int getNumArguments()const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getNumHandles() : 0;
	}

	virtual unsigned int getNumConnectors()const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getNumConnectors() : 0;
	}

	virtual const IConnector* getConnector(unsigned int index)const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getConnector(index) : NULL;
	}

	virtual const IConnector* getConnectorByName(const char* name)const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getConnectorByName(name) : NULL;
	}

	virtual unsigned int getNumCollisionShells()const{
		const T2 *r = obj.getRootNode<true>();
		return (r != NULL) ? r->getNumCollisionShells() : 0;
	}

	virtual const ICollisionShell* getCollisionShell(unsigned int i)const{
		const T2 *r = obj.getRootNode<true>();
		return (r != NULL) ? r->getCollisionShell(i) : NULL;
	}

	virtual bool hasLights()const{
		const T2 *r = obj.getRootNode<false>();
		return r != NULL ? r->hasLights() : false;
	}

	virtual unsigned int getNumBones()const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getNumReplaceableBones() : 0;
	}

	virtual unsigned int getBoneIdByName(const char *name)const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getReplaceableBoneIdByName(name) : model::INVALID_BONE_ID;
	}
	
	virtual const char* getBoneName(unsigned int id)const{
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getReplaceableBoneName(id) : NULL;
	}
	
	virtual const osg::Matrixd& getBoneMatrix(unsigned int id)const{
		static const osg::Matrixd zero(	0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0);
		
		const T2 *r = obj.getRootNode<false>();
		return (r != NULL) ? r->getReplaceableBoneMatrix(id) : zero;
	}

	virtual const char* testWithSegment(const osg::Vec3d &a, const osg::Vec3d &b, osg::Vec3d &p, osg::Vec3d &n, const osg::Matrixd &modelPos, ModelParams& params = dummyModelParams){
		forceLoad();
		const T2 *r = obj.getRootNode<true>();
		if(r == NULL) return NULL;
		
		unsigned int nArgs = getNumArguments();
		if(params.parameters.size() < nArgs) params.parameters.resize(nArgs);
		return (r != NULL) ? r->testWithSegment(a, b, p, n, modelPos, params.parameters, params.bones.empty() ? NULL : &params.bones.front(), params.bones.size()) : NULL;
	}
};

}
#endif
