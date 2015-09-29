#ifndef ED_MODEL_IGRAPHMODEL_H
#define ED_MODEL_IGRAPHMODEL_H

#include "ModelLoadState.h"
#include "edModel/IModel.h"
#include "Graphics/Camera.h"
#include "Graphics/Context.h"
#include "Graphics/Pipeline.h"
#include "edModel/FlatShadowsParams.h"
#include "edModel/InstanceParams.h"
#include "edterrain4/lPointOnSurface.h"
#include "osg/BoxUtils.h"
#ifndef EDGE
#include "Resourcer/FrameMemoryHeap.h"
#else
#include "Graphics/FrameMemoryHeap.h"
#endif

namespace edge
{
class ILightVisitor;
}

namespace model
{

/// Common interface for graphics models.
class IGraphModel
{
protected:
	/// Creates copy of given InstanceParams object in frame heap.
	inline InstanceParams* cloneInstanceParams(const InstanceParams &ip)const;
	/// Creates new FlatShadowInfo object in frame heap.
	inline FlatShadowInfo* createFlatShadowInfo(const osg::Vec3d& p, const osg::Vec3f& n, float shadowStrength);
	/// Returns if pass is suitable to render flat shadows.
	inline bool dropFlatShadow(const edge::Context& ctx)const;

	/// Renders flat shadow.
	inline void drawFlatShadow(const osg::Matrixd& modelPos, const osg::BoundingBox &bb, const osg::Vec3d& c, const osg::Vec3f& n, float blend, edge::Context& ctx, const IModelParams *params);
public:
	virtual ~IGraphModel() {}

	/// Sends model pieces to pipeline.
	/// The size of \param params->transformMatrices must have enough size to store all matrices. Minimal size of array may be got from getNumTransformNodes method.
	virtual void render(const osg::Matrixd& modelPos,edge::Context& context,edge::IQueue& opaque,edge::IQueue& transparent,InstanceParams *params)=0;
	
	/// Sends model pieces to pipeline. 
	inline void render(const osg::Matrixd& modelPos,edge::Context& context,edge::IQueue& opaque,edge::IQueue& transparent, const IModelParams *params);
	
	/// Sends model pieces to pipeline. 
	/// Model must not have any arguments!
	inline void render(const osg::Matrixd& modelPos,edge::Context& context,edge::IQueue& opaque,edge::IQueue& transparent);

	/// Cut model by frustum and render for far and near camera. 
	inline void cullAndRender(const osg::Matrixd& modelPos, edge::Context& ctx, const IModelParams *params = nullptr);
	/// Cut model by frustum and render flat shadows only
	inline void cullAndRenderFlatShadowOnly(const osg::Matrixd& modelPos, edge::Context& ctx, const osg::Vec3d& shadowPos, const osg::Vec3f& shadowNormal, const IModelParams *params = nullptr);
	/// Cut model by frustum and render together with flat shadow for far and near camera. 
	inline void cullAndRenderWithFlatShadow(const osg::Matrixd& modelPos, edge::Context& ctx, const IModelParams *params = nullptr, bool drapOnWater = true);
	/// Cut model by frustum and render together with flat shadow for far and near camera. 
	inline void cullAndRenderWithFlatShadow(const osg::Matrixd& modelPos, edge::Context& ctx, const osg::Vec3d& shadowPos, const osg::Vec3f& shadowNormal, const IModelParams *params = nullptr);

	/// Returns lights of models.
	virtual void getLights(edge::ILightVisitor& lv, const osg::Matrixd& modelPos, const IModelParams *params = nullptr)const = 0;

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

	/// Creates new InstanceParams object in frame heap.
	inline InstanceParams* createInstanceParams(const IModelParams *params)const;
};
}

#include "inl/IGraphModel.inl"

#endif
