/*
Преобразование типов gGeometry в osg и обратно
*/

#ifndef __gGeometry_osg_h__
#define __gGeometry_osg_h__

#include "Modeler/gGeometry.h"
#include "osg/BoundingBox"

inline void gGeometry2osg(osg::BoundingBox& dst, const gBox& src)
{
	dst._min.x() = src.x;
	dst._min.y() = src.y;
	dst._min.z() = src.z;
	dst._max.x() = src.X;
	dst._max.y() = src.Y;
	dst._max.z() = src.Z;
}
inline void osg2gGeometry(gBox& dst, const osg::BoundingBox& src)
{
	dst.x = src._min.x();
	dst.y = src._min.y();
	dst.z = src._min.z();
	dst.X = src._max.x();
	dst.Y = src._max.y();
	dst.Z = src._max.z();

	dst.R  = src.radius();
	dst.Distance = 0;
}

#endif
