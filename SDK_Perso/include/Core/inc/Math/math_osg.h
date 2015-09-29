/*
Преобразование типов math в osg и обратно
*/

#ifndef __math_osg_h__
#define __math_osg_h__

#include "osg/Matrixf"


inline void math2osg( osg::Vec3d& dst, const dPoint& src)
{
	dst.set(src.x, src.y, src.z);
}

inline osg::Vec3d math2osg(const dPoint& src)
{
	return osg::Vec3d(src.x, src.y, src.z);
}

inline void math2osg( osg::Vec3d& dst, const cVector& src)
{
	dst.x() = src.x;
	dst.y() = src.y;
	dst.z() = src.z;
}

inline osg::Vec3f math2osg(const cVector& src)
{
	return osg::Vec3f(src.x, src.y, src.z);
}

inline void osg2math( cVector& dst, const osg::Vec3d& src)
{
	dst.x = (float)src.x();
	dst.y = (float)src.y();
	dst.z = (float)src.z();
}

inline void math2osg( osg::Matrixd &dst, const cPosition &src)
{
	dst(0, 0) = src.x.x;
	dst(0, 1) = src.x.y;
	dst(0, 2) = src.x.z;
	dst(0, 3) = 0;

	dst(1, 0) = src.y.x;
	dst(1, 1) = src.y.y;
	dst(1, 2) = src.y.z;
	dst(1, 3) = 0;

	dst(2, 0) = src.z.x;
	dst(2, 1) = src.z.y;
	dst(2, 2) = src.z.z;
	dst(2, 3) = 0;

	dst(3, 0) = src.p.x;
	dst(3, 1) = src.p.y;
	dst(3, 2) = src.p.z;
	dst(3, 3) = 1;
}

inline void osg2math( cPosition& dst, const osg::Matrixd& src)
{
	dst.x.x = (float)src(0, 0);
	dst.x.y = (float)src(0, 1);
	dst.x.z = (float)src(0, 2);

	dst.y.x = (float)src(1, 0);
	dst.y.y = (float)src(1, 1);
	dst.y.z = (float)src(1, 2);

	dst.z.x = (float)src(2, 0);
	dst.z.y = (float)src(2, 1);
	dst.z.z = (float)src(2, 2);

	dst.p.x = (float)src(3, 0);
	dst.p.y = (float)src(3, 1);
	dst.p.z = (float)src(3, 2);
}

#endif
