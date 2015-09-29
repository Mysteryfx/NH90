namespace render
{
namespace detail
{
	inline osg::Polytope convert_to_cameraspace(osg::Polytope polytope, const osg::Matrixd& invView)
	{
		polytope.transformProvidingInverse(invView);
		return polytope;
	}

	inline osg::Polytope convert_to_worldspace(osg::Polytope polytope, const osg::Vec3f& origin)
	{
		auto* planes = polytope.getPlaneList();
		for(size_t i = 0; i < polytope.getPlaneCount(); ++i)
		{
			planes[i][3] -= planes[i].dotProductNormal(origin);
		}
		return polytope;
	}

	inline void setNearFarToPerspectiveMatrix(osg::Matrixf &perspProjLH, double zNear, double zFar)
	{
#ifndef EDGE
		ed::setNearFarToPerspectiveLHMatrix(perspProjLH, zNear, zFar);
#else
		double fovy, aspectRatio, tmp;
		perspProjLH.getPerspective(fovy, aspectRatio, tmp, tmp);
		perspProjLH.makePerspective(fovy, aspectRatio, zNear, zFar);
#endif
	}

	inline void setNearFarToOrthoMatrix(osg::Matrixf &orthProj, double zNear, double zFar)
	{
#ifndef EDGE
 		ed::setNearFarToOrthoLHMatrix(orthProj, zNear, zFar);
#else
		double left, right, bottom, top, tmp;
		orthProj.getOrtho(left, right, bottom, top, tmp, tmp);
		orthProj.makeOrtho(left, right, bottom, top, zNear, zFar);
#endif
	}

	inline void getNearFarFromPerspectiveMatrix(const osg::Matrixf &perspProj, double& zNear, double& zFar)
	{
#ifndef EDGE
 		ed::getNearFarFromPerspectiveLHMatrix(perspProj, zNear, zFar);
#else
		double fovy, aspectRatio;
		perspProj.getPerspective(fovy, aspectRatio, zNear, zFar);
#endif
	}

	inline void getNearFarFromOrthoMatrix(const osg::Matrixf &orthProj, double& zNear, double& zFar)
	{
#ifndef EDGE
		ed::getNearFarFromOrthoLHMatrix(orthProj, zNear, zFar);
#else
		double left, right, bottom, top;
		orthProj.getOrtho(left, right, bottom, top, zNear, zFar);
#endif
	}
}

inline Camera::Camera(const osg::Matrixf& view, const osg::Matrixf& proj, const osg::Vec3f& origin, enProjection type, Camera* parent):
	mView(view), mProj(proj), mOrigin(origin), mType(type), mParentCamera(parent)
{
	mInvView = osg::Matrixd::inverse(mView);
	ed::setToUnitFrustumDX(mPolytopeOrigin);
	mPolytopeOrigin.transformProvidingInverse(mView*mProj);

	mPolytope = applySpace(mPolytopeOrigin, render::SP_WORLDSPACE);
	mPolytopeCameraSpace = applySpace(mPolytopeOrigin, render::SP_CAMERASPACE);
}

inline Camera::Camera(const osg::Polytope& polytope, const osg::Vec3f origin, Camera* parent):
	mOrigin(origin), mPolytopeOrigin(polytope), mType(PR_CUSTOM), mParentCamera(parent)
{
	mPolytope = applySpace( mPolytopeOrigin, render::SP_WORLDSPACE);
	mPolytopeCameraSpace = applySpace( mPolytopeOrigin, render::SP_CAMERASPACE);
}

inline Camera Camera::makePerpective(const osg::Matrixf& view, const osg::Matrixf& proj, const osg::Vec3f& origin, Camera* parent)
{
	return Camera(view, proj, origin, enProjection::PR_PERSP, parent);
}

inline Camera Camera::makeOrtho(const osg::Matrixd& view, const osg::Matrixd& proj, const osg::Vec3f& origin, Camera* parent)
{
	return Camera(view, proj, origin, enProjection::PR_ORTHO, parent);
}

inline Camera Camera::makeCustom(const osg::Polytope& polytope, const osg::Vec3f& origin, Camera* parent)
{
	return Camera(polytope, origin, parent);
}

inline enProjection Camera::projectionType() const
{
	return mType;
}

inline osg::Matrixd Camera::position(enSpace space) const
{
	if (space == SP_WORLDSPACE){
		return mInvView * osg::Matrixd::translate(mOrigin);
	}
	if (space == SP_CAMERASPACE){
		return osg::Matrixd();
	}
	return mInvView;
}

inline osg::Matrixd Camera::view(enSpace space) const
{	
	if (space == SP_WORLDSPACE){		
		return osg::Matrixd::translate(-mOrigin) * mView;
	}
	if (space == SP_CAMERASPACE){
		return osg::Matrixd();
	}
	return mView;
}			

inline osg::Matrixd Camera::proj() const
{
	return mProj;
}

inline double Camera::nearPlane() const
{
//	assert(enProjection::PR_CUSTOM != projectionType());
	if (enProjection::PR_CUSTOM == projectionType())
		return 0;

	double zNear = 0, zFar = 0;
	
	if (projectionType() == enProjection::PR_PERSP){
		detail::getNearFarFromPerspectiveMatrix(mProj, zNear, zFar);
	}
	else 
	if (projectionType() == enProjection::PR_ORTHO) {
		detail::getNearFarFromOrthoMatrix(mProj, zNear, zFar);
	}
	return zNear;
}

inline double Camera::farPlane() const
{
//	assert(enProjection::PR_CUSTOM != projectionType());
	if (enProjection::PR_CUSTOM == projectionType())
		return 0;

	double zNear = 0, zFar = 0;

	if (projectionType() ==  enProjection::PR_PERSP)
	{
		detail::getNearFarFromPerspectiveMatrix(mProj, zNear, zFar);
	}
	else 
	if(projectionType() == enProjection::PR_ORTHO)
	{
		detail::getNearFarFromOrthoMatrix(mProj, zNear, zFar);			
	}
	return zFar;
}

inline double Camera::fovY() const
{
	assert(mType == enProjection::PR_PERSP);
	return atanf(1.f / mProj(1, 1))*2.0;
}

inline double Camera::aspect() const
{
	assert(mType != enProjection::PR_CUSTOM);
	return (1.f / mProj(0, 0))* mProj(1, 1);
}

inline const osg::Vec3f Camera::origin() const
{
	return mOrigin;
}

inline osg::Polytope Camera::applySpace(const osg::Polytope& polytope, enSpace space) const
{
	if (space==SP_WORLDSPACE){
		return detail::convert_to_worldspace(mPolytopeOrigin, mOrigin);
	}
	if (space == SP_CAMERASPACE){
		return detail::convert_to_cameraspace(mPolytopeOrigin, mInvView);
	}
	return mPolytopeOrigin;
}

inline const osg::Polytope& Camera::polytope(enSpace space) const
{	
	switch( space)
	{
		case SP_WORLDSPACE:
			return mPolytope;
		case SP_CAMERASPACE:
			return mPolytopeCameraSpace;
		case SP_WORLDSPACEORIGIN:
			return mPolytopeOrigin;
	}
	return mPolytopeOrigin;
}

inline osg::Matrixd Camera::proj(double znear, double zfar) const
{
	if (PR_CUSTOM == projectionType())
		return mProj;
	
	osg::Matrixf proj = mProj;
	if (projectionType() == PR_PERSP)
	{		
		detail::setNearFarToPerspectiveMatrix(proj, znear, zfar);
	}
	else {
		detail::setNearFarToOrthoMatrix(proj, znear, zfar);
	}
	return proj;
}



inline osg::Polytope Camera::polytope(double znear, double zfar, enSpace space) const
{
//	assert(PR_CUSTOM != projectionType());
	
	osg::Polytope polytope;
	ed::setToUnitFrustumDX(polytope);
	polytope.transformProvidingInverse(view(space) * this->proj(znear, zfar));
	return polytope;

	//osg::Polytope ptest = ed::setFarPlaneDistance(mPolytope, zfar);
	//ptest = ed::setNearPlaneDistance(ptest, znear);
}

// Камера верхнего уровня
inline const Camera* Camera::parentCamera() const
{
	return mParentCamera;
}
inline void Camera::dump( IDump* pDump) const
{
	pDump->dump("  camera 0x%x\n", this);
	if( parentCamera())
		pDump->dump("  CHILD of 0x%x\n", parentCamera());
	switch( projectionType())
	{
	case PR_PERSP:
		pDump->dump("  PERSP\n");break;
	case PR_ORTHO:
		pDump->dump("  ORTHO\n");break;
	case PR_CUSTOM:
		pDump->dump("  CUSTOM\n");break;
	}
	auto pos = this->position();
	pDump->dump("  pos={%f, %f, %f}\n", pos.getTrans().x(), pos.getTrans().y(), pos.getTrans().z());
	pDump->dump("  origin={%f, %f, %f}\n", origin().x(), origin().y(), origin().z());
	osg::Vec3d dir = pos.getAxis(2);
#ifndef EDGE
	dir = -dir;
#endif
	pDump->dump("  dir={%f, %f, %f}\n", dir.x(), dir.y(), dir.z());
	pDump->dump("  near=%f, far=%f\n", nearPlane(), farPlane());
	if( projectionType()==PR_PERSP)
		pDump->dump("  fovY=%f, aspect=%f\n", fovY(), aspect());
	else
	if (projectionType() == PR_ORTHO)
		pDump->dump("  aspect=%f\n", aspect());
}

inline bool operator ==(const Camera& lhs, const Camera& rhs)
{
	if (lhs.projectionType() == render::PR_CUSTOM)
		return false;

	if (lhs.projectionType() != rhs.projectionType())
		return false;
	if (lhs.view() != rhs.view())
		return false;
	if (lhs.proj() != rhs.proj())
		return false;
	if (lhs.origin() != lhs.origin())
		return false;

	return true;
}

}//namespace render
