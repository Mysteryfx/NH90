#ifndef OSG_TRIANGLEUTILS_H
#define OSG_TRIANGLEUTILS_H

#include <osg/Vec2d>
#include <osg/Vec3d>

#include <ed/vector.h>

#include <cmath>
#include <stdint.h>

namespace ed
{
// площадь тр-ка
inline double triangleArea(const osg::Vec2d& v0, const osg::Vec2d& v1, const osg::Vec2d& v2)
{
	osg::Vec3d a(v0.x()-v2.x(), 0, v0.y()-v2.y());
	osg::Vec3d b(v1.x()-v2.x(), 0, v1.y()-v2.y());
	return std::abs( (a^b).length() / 2.0);
}
inline double triangleArea(const osg::Vec3d& v0, const osg::Vec3d& v1, const osg::Vec3d& v2)
{
	osg::Vec3d a = v0-v2;
	osg::Vec3d b = v1-v2;
	return std::abs( (a^b).length() / 2.0);
}

// площадь полигона
inline double polygonArea(const ed::vector<osg::Vec3d>& verts) 
{
	int vc = (int)verts.size();
	osg::Vec3d sum(0, 0, 0);
	for(int v=2; v<vc; v++)
	{
		osg::Vec3d cross = (verts[v]-verts[0])^(verts[v-1]-verts[0]);
		sum += cross;
	}
	double area = sum.length();
	return area;
}


/**
 * isPointInsideTriangle - попадает ли точка в треугольник.
 *
 * @param point точка
 * @param a первая точка треугольника
 * @param b вторая точка треугольника
 * @param c третья точка треугольника
 * @param barycentric барицентрические координаты точки в этом треугольнике
 * @param threshold погрешность. Если поставить много, то будут захватываться точки
 *        которые на самом деле в этот полигон не попадают и наоборот.
 */
bool isPointInsideTriangle(const osg::Vec2d& point, const osg::Vec2d& a, const osg::Vec2d& b, const osg::Vec2d& c, osg::Vec3d& barycentric, double threshold = 0.001);

/**
 * isPointInsideTriangle - попадает ли точка в треугольник. 2D
 * Проверка производится в плоскости OXZ координата y игнорируется
 *
 * @param point точка
 * @param a первая точка треугольника
 * @param b вторая точка треугольника
 * @param c третья точка треугольника
 * @param barycentric барицентрические координаты точки в этом треугольнике
 * @param threshold погрешность. Если поставить много, то будут захватываться точки
 *        которые на самом деле в этот полигон не попадают и наоборот.
 */
bool isPointInsideTriangle(const osg::Vec3d& point, const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& c, osg::Vec3d& barycentric, double threshold = 0.001);

/**
 * Лежит ли точка p в окружности, описанной вокруг треугольника a, b, c
 * @return > 0 - точка p вне окружности, 0 - точка p на окружности, < 0 - точка P внутри окружности
 */
inline double inCircle(const osg::Vec3d& a, const osg::Vec3d& b, const osg::Vec3d& c, const osg::Vec3d& p)
{
	const double& x0 = p.z();
	const double& x1 = a.z();
	const double& x2 = b.z();
	const double& x3 = c.z();

	const double& y0 = p.x();
	const double& y1 = a.x();
	const double& y2 = b.x();
	const double& y3 = c.x();

	double s1 = x1 * x1 + y1 * y1;
	double s2 = x2 * x2 + y2 * y2;
	double s3 = x3 * x3 + y3 * y3;

	double A = x1 * y2 + y1 * x3 + x2 * y3 - y2 * x3 - y1 * x2 - x1 * y3;
	double B = s1 * y2 + y1 * s3 + s2 * y3 - y2 * s3 - y1 * s2 - s1 * y3;
	double C = s1 * x2 + x1 * s3 + s2 * x3 - x2 * s3 - x1 * s2 - s1 * x3;

	double x = B / (2.0 * A);
	double y = -C / (2.0 * A);
	double rr = (x - x1) * (x - x1) + (y - y1) * (y - y1);

	double result = (x0 - x) * (x0 - x) + (y0 - y) * (y0 - y) - rr;

	return result;
}

//================================================================================
// signedPolygonAreaOXZ
//    Площадь полигона (работает и для невыпуклых), образуемого проекциями точек 
//    на плоскость OXZ. Координата y игнорирется. Площадь возвращается со знаком
//    минус если точки завёрнуты против часовой стрелки. В Indexed версию функции
//    передаётся массив всех вершин и индексы вершин, образующих полигон.
//================================================================================
double signedPolygonAreaOXZ(const ed::vector<osg::Vec3d> &vertices);
double signedPolygonAreaOXZIndexed(const ed::vector<osg::Vec3d> &vertices, const ed::vector<int> &indicies);

//================================================================================
// polygonAreaOXZ
//    Площадь полигона (работает и для невыпуклых), образуемого проекциями точек
//    на плоскость OXZ. Координата y игнорирется. Всегда больше нуля.
//================================================================================
inline double polygonAreaOXZ(const ed::vector<osg::Vec3d> &vertices)
{
	return std::abs(ed::signedPolygonAreaOXZ(vertices));
}

inline double polygonAreaOXZIndexed(const ed::vector<osg::Vec3d> &vertices, const ed::vector<int> &indicies)
{
	return std::abs(ed::signedPolygonAreaOXZIndexed(vertices, indicies));
}

//================================================================================
// clockwise
//    Возвращает true если проекции точек на плоскость OXZ завёрнуты 
//    по часовой стрелке.
//================================================================================
inline bool clockwise(const ed::vector<osg::Vec3d> &vertices)
{
	return ed::signedPolygonAreaOXZ(vertices) > 0.0;
}

inline bool clockwiseIndexed(const ed::vector<osg::Vec3d> &vertices, const ed::vector<int> &indicies)
{
	return ed::signedPolygonAreaOXZIndexed(vertices, indicies) > 0.0;
}

//================================================================================
// circumcircleCenterOXZ
//    Центр окружности, описанной вокруг треугольника [v0, v1, v2]
//    Координата y игнорируется. При треугольниках, близких к вырожденным в линию,
//    радиус описанной окружности стремится к бесконечности, а её центр становится
//    неопределённым поэтому всегда проверяйте результат данной функции либо не
//    запихивайте в неё "кривые" треугольники.
//================================================================================
osg::Vec3d circumcircleCenterOXZ(const osg::Vec3d &v0, const osg::Vec3d &v1, const osg::Vec3d &v2);

//================================================================================
// isQuadConvex
//    Является ли четырёхугольник [a, b, c, d] выпуклым
//    threshold - синус порогового угла. если угол в вершине меньше порогового, то
//                считаем, что вершины лежат на одной линии. По-дефолту 
//                ставьте 0.02 это примерно 1 градус.
//
//    returns
//       1 - строго выпуклый
//       0 - есть подрядидущие вершины на одной линии
//      -1 - вообще ниразу не выпуклый
//================================================================================
int isQuadConvex(const osg::Vec3d &a, const osg::Vec3d &b, const osg::Vec3d &c, const osg::Vec3d &d, double threshold);

//Расстояние от точки до треугольника в 3D
//  p - точка
//  v1, v2, v3 - треугольник
double pointToTriangleDistance3D(const osg::Vec3d& p, const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& v3, double threshhold = 1e-5);

/**
 * Расстояние от точки до треугольника в 2D
 *	p - точка
 *  v1, v2, v3 - треугольник
 *  barycentric(out) - барицентрические координаты, точки на треугольнике ближайшей к p
 */
double pointToTriangleDistance2D(
	const osg::Vec3d& p, 
	const osg::Vec3d& v1, 
	const osg::Vec3d& v2, 
	const osg::Vec3d& v3,
	osg::Vec3d& barycentric,
	double epsilon = 1e-5);
}

#endif