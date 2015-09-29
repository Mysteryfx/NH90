#include "edModel/IGraphModel.h"

#ifndef EDGE
#include "Modeler/ModelParamsImpl.h"
#else
#include "Graphics/ModelParamsImpl.h"
#endif

inline model::InstanceParams* model::IGraphModel::createInstanceParams(const IModelParams *params)const
{
	InstanceParams *ip = nullptr;
	if(params == nullptr){
		static const ModelParamsImpl mpi;
		ip = new(frame_heap) InstanceParams(mpi);
	}else{
		ip = new(frame_heap) InstanceParams(*params);
	}
	
	unsigned int nMatrices = base()->getNumTransformNodes();
	ip->transformMatrices = new(frame_heap) osg::Matrixd[nMatrices];
	ip->nMatrices = nMatrices;
	return ip;
}

inline model::InstanceParams* model::IGraphModel::cloneInstanceParams(const InstanceParams &ip)const
{
	InstanceParams *p = new(frame_heap) InstanceParams(ip);
	return p;
}

inline model::FlatShadowInfo* model::IGraphModel::createFlatShadowInfo(const osg::Vec3d& p, const osg::Vec3f& n, float shadowStrength)
{
	FlatShadowInfo* fsi = new(frame_heap) FlatShadowInfo(p, n, shadowStrength);
	return fsi;
}

inline bool model::IGraphModel::dropFlatShadow(const edge::Context& ctx)const
{
	return (ctx.getPass()->getType() == edge::Pass::LOCKON || ctx.getPass()->getType() == edge::Pass::DEFERRED);
}

inline void model::IGraphModel::render(const osg::Matrixd& modelPos,edge::Context& context,edge::IQueue& opaque,edge::IQueue& transparent, const IModelParams *params)
{
	InstanceParams *ip = createInstanceParams(params);
	render(modelPos, context, opaque, transparent, ip);
}

inline void model::IGraphModel::render(const osg::Matrixd& modelPos,edge::Context& context,edge::IQueue& opaque,edge::IQueue& transparent)
{
	InstanceParams *ip = createInstanceParams(nullptr);
	render(modelPos, context, opaque, transparent, ip);
}

inline void model::IGraphModel::cullAndRender(const osg::Matrixd& modelPos, edge::Context& ctx, const IModelParams *params)
{
	const edge::Camera &cam = *ctx.getCamera();
	const osg::BoundingBoxf& box = base()->getBoundingBox();

	InstanceParams *ip = NULL;

	osg::Polytope pt;
#ifdef EDGE_SPLIT_CAMERA
	const osg::Polytope& v1=cam.getFrustum(edge::CAMERA_FAR);
	pt.setAndTransformProvidingInverse(v1,modelPos);
	if(pt.contains(box))
	{
		ip = createInstanceParams(params);
		render(modelPos, ctx, *ctx.getPipeline()->far_opaque, *ctx.getPipeline()->far_transparent, ip);
	}

	const osg::Polytope& v2=cam.getFrustum(edge::CAMERA_NEAR);
	pt.setAndTransformProvidingInverse(v2,modelPos);
	if(pt.contains(box))
	{
		if(ip == NULL){
			ip = createInstanceParams(params);
		}
		render(modelPos, ctx, *ctx.getPipeline()->near_opaque, *ctx.getPipeline()->near_transparent, ip);
	}
#else
	const osg::Polytope& v1=cam.getFrustum();
	pt.setAndTransformProvidingInverse(v1,modelPos);
	if(pt.contains(box))
	{
		ip = createInstanceParams(params);
		edge::IQueue* opaque = ctx.getPipeline()->getQueue(edge::Q_OPAQUE);
		edge::IQueue* transparent = ctx.getPipeline()->getQueue(edge::Q_TRANSPARENT);
		if(opaque && transparent)
			render(modelPos, ctx, *opaque, *transparent, ip);
	}
#endif
}

/// Cut model by frustum and render flat shadows only
inline void model::IGraphModel::cullAndRenderFlatShadowOnly(const osg::Matrixd& modelPos, edge::Context& ctx, const osg::Vec3d& shadowPos, const osg::Vec3f& shadowNormal, const IModelParams *params)
{
	// flat shadow
	if(!dropFlatShadow(ctx)) return;

	const osg::BoundingBoxf& box = base()->getBoundingBox();

	// get sun dir
	edge::lightarray_const_ref_t ld = edge::ILightVisitor::instance()->getDirLights();
	if(ld.empty()) return;

	const osg::Vec3d &sunDir = ld[0].dir;

	// box
	osg::BoundingBox bb(ed::transformBbox(box, modelPos));
	// camera
	const osg::Vec3d mPos((bb._min.x() + bb._max.x()) * 0.5, (bb._min.y() + bb._max.y()) * 0.5, (bb._min.z() + bb._max.z()) * 0.5);

	osg::Vec3d c(shadowPos);
	const osg::Vec3f &n(shadowNormal);

	if(std::abs(n.y()) < FLAT_SHADOW_NORMAL_MAX_VERT_BIAS) return;

	double cosSunDir = sunDir * n;
	if(cosSunDir < MIN_FLAT_SHADOW_ANGLE) return;

	// calc density
	double dist = ed::distance(mPos, c);
	if(dist > FLAT_SHADOW_VISIBILITY_DIST) return;

	float blend = calcFlatShadowAttenuation(cosSunDir, ctx.isOvercast());
	
	// rotate|scale
	ed::rotateBbox(box, bb, modelPos);

	// expand box by sun
	bb._min /= cosSunDir;
	bb._max /= cosSunDir;

	// move to shadow position
	ed::moveBboxTo(bb, c);

	drawFlatShadow(modelPos, bb, c, n, blend, ctx, params);
}

inline void model::IGraphModel::drawFlatShadow(const osg::Matrixd& modelPos, const osg::BoundingBox &bb, const osg::Vec3d& c, const osg::Vec3f& n, float blend, edge::Context& ctx, const IModelParams *params)
{
	const edge::Camera &cam = *ctx.getCamera();

	InstanceParams *ip = NULL;
#ifdef EDGE_SPLIT_CAMERA
	const osg::Polytope& v1=cam.getFrustum(edge::CAMERA_FAR);
	if(v1.contains(bb))
	{
		ip = createInstanceParams(params);
		ip->flatShadowInfo = createFlatShadowInfo(c, n, blend);

		render(modelPos, ctx, *ctx.getPipeline()->far_flat_shadows_opaque, *ctx.getPipeline()->far_flat_shadows_transparent, ip);
	}

	const osg::Polytope& v2=cam.getFrustum(edge::CAMERA_NEAR);
	if(v2.contains(bb))
	{
		if(ip == NULL){
			ip = createInstanceParams(params);
			ip->flatShadowInfo = createFlatShadowInfo(c, n, blend);
		}else{
			ip = cloneInstanceParams(*ip);
			ip->flatShadowInfo = createFlatShadowInfo(c, n, blend);
		}
		render(modelPos, ctx, *ctx.getPipeline()->near_flat_shadows_opaque, *ctx.getPipeline()->near_flat_shadows_transparent, ip);
	}
#else
	const osg::Polytope& v1=cam.getFrustum();
	if(v1.contains(bb))
	{
		ip = createInstanceParams(params);
		ip->flatShadowInfo = createFlatShadowInfo(c, n, blend);

		edge::IQueue* flat_shadows_opaque = ctx.getPipeline()->getQueue(edge::Q_FLAT_SHADOWS_OPAQUE);
		edge::IQueue* flat_shadows_transparent = ctx.getPipeline()->getQueue(edge::Q_FLAT_SHADOWS_TRANSPARENT);
		render(modelPos, ctx, *flat_shadows_opaque, *flat_shadows_transparent, ip);
	}
#endif
}

inline void model::IGraphModel::cullAndRenderWithFlatShadow(const osg::Matrixd& modelPos, edge::Context& ctx,const IModelParams *params, bool drapOnWater)
{
	cullAndRender(modelPos, ctx, params);

	// flat shadow
	if(!dropFlatShadow(ctx)) return;

	const osg::BoundingBoxf& box = base()->getBoundingBox();	

	// get sun dir
	edge::lightarray_const_ref_t ld = edge::ILightVisitor::instance()->getDirLights();
	if(ld.empty()) return;

	const osg::Vec3d &sunDir = ld[0].dir;

	// box
	osg::BoundingBox bb(ed::transformBbox(box, modelPos));
	// camera
	const osg::Vec3d mPos((bb._min.x() + bb._max.x()) * 0.5, (bb._min.y() + bb._max.y()) * 0.5, (bb._min.z() + bb._max.z()) * 0.5);

	landscape::lPointOnSurface lpos;
	osg::Vec3d c;
	// we need use center of bounding box due to trees. As their positions may be in common triangles point.
	osg::Vec3d a(mPos), b(a);
	a -= sunDir * ed::distance(box._min, box._max) * (-0.5);
	b += sunDir * (-FLAT_SHADOW_VISIBILITY_DIST);

	if(!lpos.intersection(a, b, &c)){
		return;
	}

	if(!drapOnWater){
		if((lpos.getType() & (smaSea | smaLake | smaRiver)) != 0){
			return;
		}
	}
	osg::Vec3f n = lpos.getNormal();

	if(std::abs(n.y()) < FLAT_SHADOW_NORMAL_MAX_VERT_BIAS) return;

	double cosSunDir = sunDir * n;
	if(cosSunDir < MIN_FLAT_SHADOW_ANGLE) return;

	// calc density
	float blend = calcFlatShadowAttenuation(cosSunDir, ctx.isOvercast());

	// rotate|scale
	ed::rotateBbox(box, bb, modelPos);

	// expand box by sun
	bb._min /= cosSunDir;
	bb._max /= cosSunDir;

	// move to shadow position
	ed::moveBboxTo(bb, c);

	drawFlatShadow(modelPos, bb, c, n, blend, ctx, params);
}

inline void model::IGraphModel::cullAndRenderWithFlatShadow(const osg::Matrixd& modelPos, edge::Context& ctx, const osg::Vec3d& shadowPos, const osg::Vec3f& shadowNormal, const IModelParams *params)
{
	cullAndRender(modelPos, ctx, params);

	// flat shadow
	if(!dropFlatShadow(ctx)) return;

	const osg::BoundingBoxf& box = base()->getBoundingBox();

	// get sun dir
	edge::lightarray_const_ref_t ld = edge::ILightVisitor::instance()->getDirLights();
	if(ld.empty()) return;

	const osg::Vec3d &sunDir = ld[0].dir;

	// box
	osg::BoundingBox bb(ed::transformBbox(box, modelPos));
	// camera
	const osg::Vec3d mPos((bb._min.x() + bb._max.x()) * 0.5, (bb._min.y() + bb._max.y()) * 0.5, (bb._min.z() + bb._max.z()) * 0.5);

	osg::Vec3d c(shadowPos);
	const osg::Vec3f &n(shadowNormal);

	if(std::abs(n.y()) < FLAT_SHADOW_NORMAL_MAX_VERT_BIAS) return;

	double cosSunDir = sunDir * n;
	if(cosSunDir < MIN_FLAT_SHADOW_ANGLE) return;

	// calc density
	double dist = ed::distance(mPos, c);
	if(dist > FLAT_SHADOW_VISIBILITY_DIST) return;

	float blend = calcFlatShadowAttenuation(cosSunDir, ctx.isOvercast());
	
	// rotate|scale
	ed::rotateBbox(box, bb, modelPos);

	// expand box by sun
	bb._min /= cosSunDir;
	bb._max /= cosSunDir;

	// move to shadow position
	ed::moveBboxTo(bb, c);

	drawFlatShadow(modelPos, bb, c, n, blend, ctx, params);
}
