#ifndef OSG_MATH_H
#define OSG_MATH_H

#include <osg/Vec2d>
#include <osg/Vec3d>

namespace ed
{

/**
 * clamp - pretty selfdescribing
 */
template <typename T> T clamp(T value, T min, T max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}

/**
 * lerp - pretty selfdescribing
 */
template <typename T> T lerp(T a, T b, double t)
{
	return a * (1 - t) + b * t;
}

/**
 * sign - sign function
 */
template <typename T> T sign(T value)
{
	if (value > 0)
		return 1;

	if (value < 0)
		return -1;

	return 0;
}

/**
 * angle2d - Возвращает разницу полярных углов векторов v2 и v1 в плоскости XZ, значение от 0 до 2 * Pi
 */
inline double angle2d(const osg::Vec3d& v1, const osg::Vec3d& v2)
{
	double theta1 = atan2(v1.z(), v1.x());
	double theta2 = atan2(v2.z(), v2.x());

	if (theta2 < theta1) 
		theta2 += 2 * M_PI;

	return (theta2 - theta1);
}

/**
 * cross2d - Векторное произведение проекций векторов на плоскость OXZ. Координата y игнорируется.
 */
inline double cross2d(const osg::Vec3d& a, const osg::Vec3d& b)
{
	return a.x() * b.z() - b.x() * a.z();
}

inline double cross2d(const osg::Vec2d& a, const osg::Vec2d& b)
{
	return a.x() * b.y() - b.x() * a.y();
}

/**
 * wrapAngle - Приводит значение угла к соответствующему значению из полуинтервала [-Pi, Pi)
 */
inline double wrapAngle(double angle)
{
	return angle - 2 * M_PI * floor((angle + M_PI) / (2 * M_PI));
}

/**
 * wrapAngle0_2Pi - Приводит значение угла к соответствующему значению из полуинтервала [0, 2*Pi)
 */
inline double wrapAngle0_2Pi(double angle)
{
	return angle - 2 * M_PI * floor(angle / (2 * M_PI));
}

}

#endif