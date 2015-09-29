#ifndef OSG_POINTUTILS_H
#define OSG_POINTUTILS_H

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

/**
 * length2d - длина проекции вектора на плоскость OXZ
 */
inline double length2d(const osg::Vec3d& vec)
{
	return sqrt(vec.x() * vec.x() + vec.z() * vec.z());
}
inline float length2d(const osg::Vec3f& vec)
{
	return sqrt(vec.x() * vec.x() + vec.z() * vec.z());
}

/**
 * length2d - длина проекции вектора на плоскость OXZ
 */
inline double length2d(const osg::Vec2d& vec)
{
	return sqrt(vec.x() * vec.x() + vec.y() * vec.y());
}

inline bool findClosestPoints2d(const ed::vector<osg::Vec3d>& source, const ed::vector<osg::Vec3d>& points, ed::vector<int>& sourceClosestToPoints)
{
	sourceClosestToPoints.resize(points.size(), -1);
	ed::vector<double> distances(points.size(), FLT_MAX);

	for (size_t i = 0; i < source.size(); i++)
	{
		const osg::Vec3d& s = source[i];

		for (size_t j = 0; j < points.size(); j++)
		{
			const osg::Vec3d& p = points[j];
			double& distance = distances[j];

			double currentDistance = ed::length2d(s - p);

			if (currentDistance < distance)
			{
				distance = currentDistance;
				sourceClosestToPoints[j] = i;
			}
		}
	}

	return true;
}

/**
 * floor - покомпонентный floor для векторов
 */
inline osg::Vec2f floor2d(const osg::Vec2f& vec)
{
	return osg::Vec2f(floor(vec.x()), floor(vec.y()));
}

inline osg::Vec3f floor3d(const osg::Vec3f& vec)
{
	return osg::Vec3f(floor(vec.x()), floor(vec.y()), floor(vec.z()));
}

inline osg::Vec2d floor2d(const osg::Vec2d& vec)
{
	return osg::Vec2d(floor(vec.x()), floor(vec.y()));
}

inline osg::Vec3d floor3d(const osg::Vec3d& vec)
{
	return osg::Vec3d(floor(vec.x()), floor(vec.y()), floor(vec.z()));
} 

inline double distance(const osg::Vec3d &v1, const osg::Vec3d &v2)
{
	double a = v1.x() - v2.x();
	double b = v1.y() - v2.y();
	double c = v1.z() - v2.z();
	return sqrt(a * a + b * b + c * c);
}

inline float distance(const osg::Vec3f &v1, const osg::Vec3f &v2)
{
	float a = v1.x() - v2.x();
	float b = v1.y() - v2.y();
	float c = v1.z() - v2.z();
	return sqrtf(a * a + b * b + c * c);
}

inline double distance2d(const osg::Vec3d &v1, const osg::Vec3d &v2)
{
	double a = v1.x() - v2.x();
	double c = v1.z() - v2.z();
	return sqrt(a * a + c * c);
}

inline float distance2d(const osg::Vec3f &v1, const osg::Vec3f &v2)
{
	float a = v1.x() - v2.x();
	float c = v1.z() - v2.z();
	return sqrtf(a * a + c * c);
}

/**
 * normalize2d - нормализованный вектор проекции вектора на плоскость OXZ
 */
inline osg::Vec3d normalize2d(const osg::Vec3d& vec)
{
	osg::Vec3d result(vec.x(), 0.0, vec.z());
	result.normalize();
	return result;
}

/**
 * normalize - нормализованный вектор
 */
template<class Vec> Vec normalize(const Vec& v)
{
	typename Vec::value_type norm = v.length();
	if (norm > 0.0)
	{
		typename Vec::value_type inv = 1.0f / norm;
		return v * inv;
	}         
	return v;
}

}

#endif
