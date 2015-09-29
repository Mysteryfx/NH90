#ifndef OSG_BOXUTILS_H
#define OSG_BOXUTILS_H

#include <algorithm>

#include <osg/Vec3f>
#include <osg/Vec3d>
#include <osg/BoundingBox>
#include <osg/Polytope>
#include <osg/Matrixd>

#include "osg/OSGMath.h"
#include "osg/IntersectionUtils.h"

namespace ed
{
//================================================================================
// distanceTo - расстояние от бокса до точки
//================================================================================
inline float distanceTo(const osg::BoundingBox& box, const osg::Vec3f& point)
{
	osg::Vec3f closestPoint(ed::clamp(point.x(), (float)box._min.x(), (float)box._max.x()),
							ed::clamp(point.y(), (float)box._min.y(), (float)box._max.y()),
							ed::clamp(point.z(), (float)box._min.z(), (float)box._max.z()));

	closestPoint -= point;
	return closestPoint.length();
}

inline double distanceTo(const osg::BoundingBox& box, const osg::Vec3d& point)
{
	osg::Vec3d closestPoint(ed::clamp(point.x(), (double)box._min.x(), (double)box._max.x()),
							ed::clamp(point.y(), (double)box._min.y(), (double)box._max.y()),
							ed::clamp(point.z(), (double)box._min.z(), (double)box._max.z()));

	closestPoint -= point;
	return closestPoint.length();
}

//================================================================================
// maxDistanceTo - расстояние от точки до максимально удаленной точки бокса 
//================================================================================
inline float maxDistanceTo(const osg::BoundingBox& box, const osg::Vec3f& point)
{
	osg::Vec3f farest(
		std::max(fabs(box.xMin() - point.x()), fabs(box.xMax() - point.x())),
		std::max(fabs(box.yMin() - point.y()), fabs(box.yMax() - point.y())),
		std::max(fabs(box.zMin() - point.z()), fabs(box.zMax() - point.z()))
		);
	return farest.length();
}

inline double maxDistanceTo(const osg::BoundingBox& box, const osg::Vec3d& point)
{
	osg::Vec3d farest(
		std::max(fabs(box.xMin() - point.x()), fabs(box.xMax() - point.x())),
		std::max(fabs(box.yMin() - point.y()), fabs(box.yMax() - point.y())),
		std::max(fabs(box.zMin() - point.z()), fabs(box.zMax() - point.z()))
		);
	return farest.length();
}

//================================================================================
// boxOfFrustum - построить BoundingBox для фрустума
//================================================================================
osg::BoundingBox boxOfFrustum(const osg::Polytope& frustum);

/// Multiply bounding box by matrix.
/// \param bbox bounding box to transform.
/// \param m transformation matrix.
osg::BoundingBox transformBbox(const osg::BoundingBox& bbox,
							   const osg::Matrixd& m);

/// Multiply bounding box by matrix.
/// \param src bounding box to transform.
/// \param dst resulting bounding box.
/// \param m transformation matrix.
void transformBbox(const osg::BoundingBox& src, osg::BoundingBox& dst, const osg::Matrixd& m);

/// Multiply bounding box by matrix, dropping translation.
/// \param src bounding box to transform.
/// \param dst resulting bounding box.
/// \param m transformation matrix.
void rotateBbox(const osg::BoundingBox& src, osg::BoundingBox& dst, const osg::Matrixd& m);

/// Moves bounding box by adding given point.
inline void moveBboxTo(osg::BoundingBox& bb, const osg::Vec3d &v){
	bb._max.x() += v.x();
	bb._max.y() += v.y();
	bb._max.z() += v.z();
	bb._min.x() += v.x();
	bb._min.y() += v.y();
	bb._min.z() += v.z();
}

/// Moves bounding box by adding given point.
inline void moveBboxTo(osg::BoundingBox& bb, const osg::Vec3f &v){
	bb._max += v;
	bb._min += v;
}

/**
 * transformFromBox - получить матрицу переводящую бокс ((-1, -1, -1), (1, 1, 1)) в заданный бокс
 */
osg::Matrixd transformFromBbox(const osg::BoundingBoxf& box);

}

#endif
