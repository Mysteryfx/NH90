#ifndef OSG_POLYLINE_H
#define OSG_POLYLINE_H

#include <cstring>

#include <osg/Vec2f>
#include <osg/Vec2d>
#include <osg/Vec3f>
#include <osg/Vec3d>
#include <math.h>
#include <float.h>
#include <ed/vector.h>

namespace ed
{
// [&](int vertexIndex)->osg::Vec3d 
template <typename Function> osg::Vec3d polyline3dVertTangent(int vert, int cvCount, const Function& f)
{
	if( cvCount<2 || vert<0 || vert>=cvCount)
		return osg::Vec3d(0, 0, 0);
	osg::Vec3d dir;
	if(vert==0)
	{
		dir = f(vert + 1) - f(vert);
		dir.normalize();
	}
	else if(vert==cvCount-1)
	{
		dir = f(vert) - f(vert-1);
		dir.normalize();
	}
	else
	{
		osg::Vec3d dir1 = f(vert) - f(vert-1);
		osg::Vec3d dir2 = f(vert) - f(vert-1);
		dir1.normalize();
		dir2.normalize();
		dir = dir1+dir2;
		dir.normalize();

		double dot = (dir1*dir);
		dot = std::max( 0.001, dot);
		dir *= 1/dot;
	}
	return dir;
}

// [&](int vertexIndex)->osg::Vec3d 
template <typename Function> osg::Vec3d polyline3dVertNormal(int vert, int cvCount, const Function& f)
{
	osg::Vec3d tangent = ed::polyline3dVertTangent(vert, cvCount, f);
	osg::Vec3d normal = tangent^osg::Vec3d(0, 1, 0);
	return normal;
}

// [&](int polyline, int vertexIndex)->osg::Vec3d 
// [&](int s1, int s2, const osg::Vec3d& respoint)->bool
template <typename FunctionPoint, typename FunctionRes> bool polylinePolylineIntersection2d(int vertCount1, int vertCount2, const FunctionPoint& fpoint, const FunctionRes& fresult)
{
	// каждый с каждым
	for(int v1=1; v1<vertCount1; v1++)
	{
		osg::Vec3d a1 = fpoint(0, v1-1);
		osg::Vec3d a2 = fpoint(0, v1);

		for(int v2=1; v2<vertCount2; v2++)
		{
			osg::Vec3d b1 = fpoint(1, v2-1);
			osg::Vec3d b2 = fpoint(1, v2);

			osg::Vec3d respoint;
			if( ed::segmentIntersection(a1, a2, b1, b2, respoint))
			{
				if( fresult(v1-1, v2-1, respoint))
					return true;
			}
		}
	}
	return false;
}

}

#endif
