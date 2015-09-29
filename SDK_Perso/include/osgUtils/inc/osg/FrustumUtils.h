#ifndef OSG_FRUSTUMUTILS_H
#define OSG_FRUSTUMUTILS_H

namespace ed
{
/**
 * setFarPlaneDistance - изменить Far plane у Frustum
 *
 * @param frustum исходный Frustum
 * @param distance куда передвинуть far plane
 */

inline osg::Polytope setFarPlaneDistance(const osg::Polytope& frustum, double distance)
{
	osg::Polytope result = frustum;

	osg::Plane& farPlane = result.getPlaneList()[5];
	farPlane = result.getPlaneList()[4];
	farPlane.flip();
	farPlane[3] += distance;
	return result;
}

/**
 * setNearPlaneDistance - изменить Near plane у Frustum
 *
 * @param frustum исходный Frustum
 * @param distance куда передвинуть far plane
 */

inline osg::Polytope setNearPlaneDistance(const osg::Polytope& frustum, double distance)
{
	osg::Polytope result = frustum;

	osg::Plane& nearPlane = result.getPlaneList()[4];
	nearPlane[3] -= distance;
	return result;
}

// osg setToUniformFrustum устанавливает near в 1 нам надо 0
inline void setToUnitFrustumDX(osg::Polytope& polytope)
{
	polytope.clear();
	polytope.add(osg::Plane(1.0f,0.0f,0.0f,1.0f)); // left plane.
	polytope.add(osg::Plane(-1.0f,0.0f,0.0f,1.0f)); // right plane.
	polytope.add(osg::Plane(0.0f,1.0f,0.0f,1.0f)); // bottom plane.
	polytope.add(osg::Plane(0.0f,-1.0f,0.0f,1.0f)); // top plane.
	polytope.add(osg::Plane(0.0f,0.0f,1.0f,0.0f)); // near plane
	polytope.add(osg::Plane(0.0f,0.0f,-1.0f,1.0f)); // far plane
}

}

#endif