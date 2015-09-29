#ifndef OSG_SEGMENTUTILS_H
#define OSG_SEGMENTUTILS_H

#include <osg/Vec2d>
#include <osg/Vec3d>
#include "ed_cmath.h"
#include <ed/vector.h>
#include "osg/PointUtils.h"
#include "osg/OSGMath.h"

namespace ed
{
	struct Segment3d
	{
		osg::Vec3d a;
		osg::Vec3d b;
		Segment3d(const osg::Vec3d& a, const osg::Vec3d& b) : a(a) , b(b) {}
	};

	//Нахождение кратчайшего растояния от точки до отрезка
double getPointToSegmentLength(
	const osg::Vec3d &v,				//Координаты точки
	const osg::Vec3d &v1,				//Первая точка отрезка
	const osg::Vec3d &v2,				//Вторая точка отрезка	
	osg::Vec3d &res,					//Ближайшая точка на отрезке
	double epsilon						//Допустимая погрешность
	);

//Нахождение кратчайшего растояния от точки до отрезка
double getSegmentClosestPoint(
	const osg::Vec2d &v,				//Координаты точки
	const osg::Vec2d &v1,				//Первая точка отрезка
	const osg::Vec2d &v2,				//Вторая точка отрезка	
	double &param,						//Ближайшая точка на отрезке
	double epsilon						//Допустимая погрешность
	);

// Сегменты совпадают?
inline bool isSegmentsEqual(const osg::Vec3d& a1, const osg::Vec3d& b1, const osg::Vec3d& a2, const osg::Vec3d& b2, double treshold=0.0001, bool* bInvert=NULL)
{
	if( (a1-a2).length()<treshold && (b1-b2).length()<treshold)
	{
		if( bInvert) *bInvert = false;
		return true;
	}
	if( (a1-b2).length()<treshold && (b1-a2).length()<treshold)
	{
		if( bInvert) *bInvert = true;
		return true;
	}
	return false;
}

/**
 * Расстояние от точки до отрезка
 * @param seg1 первая точка сегмента
 * @param seg2 вторая точка сегмента
 * @param point точка
 * @return расстояние в 2d
 */
inline double distanceToSegment(const osg::Vec3d& seg1, const osg::Vec3d& seg2, const osg::Vec3d& point)
{
	osg::Vec3d startToEnd = seg2 - seg1;
	osg::Vec3d startToPoint = point - seg1;
	startToEnd.y() = 0.0;
	startToPoint.y() = 0.0;

	double t = (startToPoint * startToEnd) / startToEnd.length2(); 

	if (!std::isfinite(t) || t <= 0.0)
		return ed::length2d(startToPoint);

	if (t >= 1.0)
		return ed::length2d(point - seg2);

	osg::Vec3d pointOnSegment = ed::lerp(seg1, seg2, t);

	return ed::length2d(pointOnSegment - point);
}

/**
 * Расстояние от точки до луча
 * @param pt первая точка луча
 * @param dir наравление
 * @param point точка
 * @return расстояние в 3d
 */
inline double distanceToRay3d(const osg::Vec3d& pt, const osg::Vec3d& dir, const osg::Vec3d& point)
{
	osg::Vec3d startToEnd = dir;
	osg::Vec3d startToPoint = point - pt;

	double t = (startToPoint * startToEnd) / startToEnd.length2(); 

	if (!std::isfinite(t) || t <= 0.0)
		return startToPoint.length();

	osg::Vec3d pointOnSegment = ed::lerp(pt, pt+dir, t);

	return (pointOnSegment - point).length();
}

/**
 * Спроектировать точку на линию
 * @param a первая точка линии
 * @param b вторая точка линии
 * @param p точка
 * @return параметрическая координата проекции на линии
 */
inline double project(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& p)
{
	osg::Vec3d ab = b - a;
	osg::Vec3d ap = p - a;
	ab.y() = 0.0;
	ap.y() = 0.0;

	double t = (ap * ab) / ap.length2();

	return t;
}

/**
 * Спроектировать точку на линию
 * @param a первая точка линии
 * @param b вторая точка линии
 * @param p точка
 * @return параметрическая координата проекции на линии
 */
inline double project(const osg::Vec2d& a, const osg::Vec2d& b, const osg::Vec2d& p)
{
	osg::Vec2d ab = b - a;
	osg::Vec2d ap = p - a;

	double t = (ap * ab) / ab.length2();

	return t;
}

// Аппроксимация полилинии 
// Алгоритм Дугласа-Пекера (Douglas-Peucker)
void Approximate_DouglasPeucker(
	ed::vector<osg::Vec2d>& verts, // модифицируемая линия
	double approctolerance
	);
// надо доделать (работает в 2d)
void Approximate_DouglasPeucker(
	ed::vector<osg::Vec3d>& verts, // модифицируемая линия
	double approctoleranceXZ, 
	double approctoleranceY
	);
}

#endif