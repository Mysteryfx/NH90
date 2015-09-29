/*
Преобразование типов graphmath в osg и обратно
*/

#pragma once

inline void math2osg( osg::Vec3d& dst, const Vector3& src)
{
	dst.set(src.x, src.y, src.z);
}
inline void osg2math( Vector3& dst, const osg::Vec3d& src)
{
	dst.x = (float)src.x();
	dst.y = (float)src.y();
	dst.z = (float)src.z();
}
inline void math2osg( osg::Vec3f& dst, const Vector3& src)
{
	dst.set(src.x, src.y, src.z);
}
inline void osg2math( Vector3& dst, const osg::Vec3f& src)
{
	dst.x = src.x();
	dst.y = src.y();
	dst.z = src.z();
}
inline void math2osg( osg::BoundingBox& dst, const Box& src)
{
	dst._min.set(src.min.x, src.min.y, src.min.z);
	dst._max.set(src.max.x, src.max.y, src.max.z);
}

