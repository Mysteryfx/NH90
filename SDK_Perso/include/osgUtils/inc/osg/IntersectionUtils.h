#ifndef OSG_INTERSECTIONUTILS_H
#define OSG_INTERSECTIONUTILS_H

#include <osg/Vec2d>
#include <osg/Vec3d>
#include <osg/Plane>
#include <osg/Polytope>
#include <osg/BoundingBox>

#include "osg/OSGMath.h"
#include "osg/PointUtils.h"
#include "osg/TriangleUtils.h"

#include "SegmentUtils.h"

namespace ed
{

// Найти точки пересечения окружности и прямой; могут совпадать, если прямая касается
bool circleLineIntersection(const osg::Vec3d& center, double radius, const osg::Vec3d& a, const osg::Vec3d& b, double& t1, double& t2, double epsilon);
// Найти точки пересечения окружности и прямой; могут совпадать, если прямая касается
bool circleSegmentIntersection(const osg::Vec3d& center, double radius, const osg::Vec3d& a, const osg::Vec3d& b, double& t1, double& t2, double epsilon);

/**
 * segmentTriangleIntersection - пересечение отрезка и треугольника в 3D
 *
 * @param a первая точка отрезка
 * @param b вторая точка отрезка
 * @param p0 первая точка треугольника
 * @param p1 вторая точка треугольника
 * @param p2 третья точка треугольника
 * @param t точка пересечения в параметрических координатах отрезка
 * @param barycentric барицентрические координаты точки пересечения в треугольнике
 * @param threshold погрешность. Если поставить много, то будут захватываться точки
 *        которые на самом деле в этот полигон не попадают и наоборот.
 */
bool segmentTriangleIntersection(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& p0, const osg::Vec3d& p1, const osg::Vec3d& p2, double& t, osg::Vec3d& barycentric, double threshold = 0.001);

/**
 * segmentTriangleIntersection - пересечение отрезка и треугольника в 3D
 *
 * @param a первая точка отрезка
 * @param b вторая точка отрезка
 * @param p0 первая точка треугольника
 * @param p1 вторая точка треугольника
 * @param p2 третья точка треугольника
 * @param t точка пересечения в параметрических координатах отрезка
 * @param n triangle normal
 * @param threshold погрешность. Если поставить много, то будут захватываться точки
 *        которые на самом деле в этот полигон не попадают и наоборот.
 */
bool segmentTriangleIntersection2(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& p0, const osg::Vec3d& p1, const osg::Vec3d& p2, double& t, osg::Vec3d &n, double threshold = 0.001);

/**
 * segmentTriangleIntersection2D - пересечение отрезка и треугольника в 2D
 *
 * @param a первая точка отрезка
 * @param b вторая точка отрезка
 * @param p0 первая точка треугольника
 * @param p1 вторая точка треугольника
 * @param p2 третья точка треугольника
 * @param t точка пересечения в параметрических координатах отрезка
 * @param barycentric барицентрические координаты точки пересечения в треугольнике
 * @param threshold погрешность. Если поставить много, то будут захватываться точки
 *        которые на самом деле в этот полигон не попадают и наоборот.
 */
bool segmentTriangleIntersection2D(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& p0, const osg::Vec3d& p1, const osg::Vec3d& p2,  double& t, osg::Vec3d& barycentric, double threshold = 0.001);

/**
 * segmentIntersection - пересечение двух отрезков
 *
 * @param a1 первая точка первого отрезка
 * @param b1 вторая точка первого отрезка
 * @param a2 первая точка второго отрезка
 * @param b2 вторая точка второго отрезка
 * @param t1 точка пересечения в параметрическом виде для первого отрезка. lerp(a1, b1, t1)
 * @param t2 точка пересечения в параметрическом виде для второго отрезка. lerp(a2, b2, t2)
 * @param погрешность
 */
bool segmentIntersection(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& c, const osg::Vec3d& d, double& t1, double& t2, double threshold = 0.001);
bool segmentIntersection(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& c, const osg::Vec3d& d, osg::Vec3d& respoint, double threshold = 0.001);
bool segmentIntersection(const osg::Vec2d& a, const osg::Vec2d& b, const osg::Vec2d& c, const osg::Vec2d& d, double& t1, double& t2, double threshold = 0.001);


/**
 * lineIntersection - пересечение двух прямых
 *
 * @param a первая точка первой прямой
 * @param b вторая точка первой прямой
 * @param c первая точка второй прямой
 * @param d вторая точка второй прямой
 * @param t1 точка пересечения в параметрическом виде для первой прямой. lerp(a, b, t1)
 * @param t2 точка пересечения в параметрическом виде для второй прямой. lerp(c, d, t2)
 * @param погрешность
 */
bool lineIntersection(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& c, const osg::Vec3d& d, double& t1, double& t2, double threshold = 0.001);
bool lineIntersection(const osg::Vec2d& a, const osg::Vec2d& b, const osg::Vec2d& c, const osg::Vec2d& d, double& t1, double& t2, double threshold = 0.001);

/**
 * trianglesIntersects - пересекаются ли треугольники
 *
 * @param a1 первая точка первого треугольника
 * @param b1 вторая точка первого треугольника
 * @param c1 третья точка первого треугольника
 * @param a2 первая точка второго треугольника
 * @param b2 вторая точка второго треугольника
 * @param c2 третья точка второго треугольника
 * @param threshold погрешность
 */
bool trianglesIntersects(const osg::Vec3d& a1, const osg::Vec3d& b1, const osg::Vec3d& c1, const osg::Vec3d& a2, const osg::Vec3d& b2, const osg::Vec3d& c2, double threshold = 0.001);


//================================================================================
// intersectPlanes - найти точку пересечения трёх плоскостей
//
// osg::Plane plane1, plane2, plane3 - три плоскости
// [out] osg::Vec3d point            - точка пересечения плоскостей
// bool return                       - true если плоскости имеют одну общую точку пересечения
//================================================================================
bool intersectPlanes(const osg::Plane& plane1, const osg::Plane& plane2, const osg::Plane& plane3, osg::Vec3d& point);

//================================================================================
// boxLineIntersection
//    Пересечение бокса и прямой, заданной двумя точками.
//    Возвращает результат в параметрическом виде. a * (1 - t) + b * t
//================================================================================
bool boxLineIntersection(const osg::BoundingBox& box, const osg::Vec3d& a, const osg::Vec3d& b, double& tMin, double& tMax);

//================================================================================
// boxSegmentIntersection
//    Пересечение бокса и отрезка, заданного двумя точками.
//    Возвращает результат в параметрическом виде. a * (1 - t) + b * t
//================================================================================
bool boxSegmentIntersection(const osg::BoundingBox& box, const osg::Vec3d& a, const osg::Vec3d& b, double* tMin=0, double* tMax=0);

/// Check intersection between bounding box and segment.
bool boxSegmentIntersect(const osg::BoundingBox& bb, const osg::Vec3f &a, const osg::Vec3f &b);

// plane-segment intersection
bool planeSegmentIntersection(const osg::Plane& plane, const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d* point=NULL, double* param=NULL);

//Проверка пересекаются ли прямые, координата 'y' игнорируется
bool lineIntersection(
   const osg::Vec3d &v11,				//Первая точка 1 прямой
   const osg::Vec3d &v12,				//Вторая точка 1 прямой
   const osg::Vec3d &v21,				//Первая точка 2 прямой
   const osg::Vec3d &v22,				//Вторая точка 2 прямой
   osg::Vec3d &res,						//out: Точка пересечения
   double epsilon = 0.01				//Допустимая погрешность
   );

//Проверка пересекаются ли прямые, координата 'y' игнорируется
bool lineIntersection(   
	const osg::Vec2d &v11,				//Первая точка 1 прямой
	const osg::Vec2d &v12,				//Вторая точка 1 прямой
	const osg::Vec2d &v21,				//Первая точка 2 прямой
	const osg::Vec2d &v22,				//Вторая точка 2 прямой
	osg::Vec2d &res,					//out: Точка пересечения
	double epsilon						//Допустимая погрешность
	);

// box intersections. for template functions
	inline bool boxIntersection(const osg::Polytope& primitive, const osg::BoundingBoxf& box)
	{
		return primitive.contains(box);
	}
	
	inline bool boxIntersection(const Segment3d& primitive, const osg::BoundingBoxf& box)
	{
		return ed::boxSegmentIntersection( box, primitive.a, primitive.b);
	}

	inline bool boxIntersection(const osg::BoundingBoxf& primitive, const osg::BoundingBoxf& box)
	{
		return primitive.intersects(box);
	}

	/**
	 * Проверяет, находится ли точка внутри замкнутого контура
	 */
	inline bool isPointInsideClosedLoop(const osg::Vec3d& point, const ed::vector<osg::Vec3d>& loop, double delta = 0.1)
	{
		if (loop.size() < 3)
			return false;

		ed::vector<osg::Vec3d> loopVertices;
		loopVertices.reserve(loop.size() + 1);

		loopVertices.push_back(loop[0]);
		for (size_t i = 1; i < loop.size(); i++)
			if (ed::length2d(loopVertices.back() - loop[i]) > delta)
				loopVertices.push_back(loop[i]);

		if (loopVertices.size() < 3)
			return false;

		if (ed::length2d(loopVertices.back() - loopVertices.front()) > delta)
			loopVertices.push_back(loopVertices.front());

		int count = 0;
		osg::Vec3d scanVector(1.0, 0.0, 0.0);

		for (int32_t i = 0; i < loopVertices.size() - 1; i++)
		{
			const osg::Vec3d& a = loopVertices[i];
			const osg::Vec3d& b = loopVertices[i + 1];

			if ((ed::length2d(a - point) < delta) || (ed::length2d(b - point) < delta))
				return true;

			double t1, t2;
			if (!ed::lineIntersection(a, b, point, point + scanVector, t1, t2))
			{
				count = 0;
				i = -1;
				scanVector.z() += 0.1;
				continue;
			}

			if ((0.0 <= t1) && (t1 <= 1.0) && (0.0 <= t2))
				count++;
		}

		return (count % 2 != 0);
	}

}

#endif
