#ifndef PGFDIRECTORY_H__
#define PGFDIRECTORY_H__
#include "../landscape3_config.h"

// Пересечение треугольника и единичного куба (-1, 1)
EDTERRAIN_API bool triangleUnitCubeIntersection(
	const osg::Vec3f& pt0, 
	const osg::Vec3f& pt1, 
	const osg::Vec3f& pt2);
EDTERRAIN_API bool triangleUnitCubeIntersection(
	const osg::Vec4f& pt0, 
	const osg::Vec4f& pt1, 
	const osg::Vec4f& pt2);

#endif