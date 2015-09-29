#ifndef __EDTERRAIN_MATH_H
#define __EDTERRAIN_MATH_H

#include "edterrain.h"
#include "osg\Vec3d"
#include "osg\Matrixd"
#include "osg\Matrixf"
#include "osg\Plane"
#include "osg\Polytope"
#include "osg\BoundingBox"
#include <ed/vector.h>

namespace offshore
{
	struct PointOnSpline;
};

//================================================================================
// sign
//================================================================================
//Удалить. Виталик
inline double sign(double value)
{
	if (value < 0.0) return -1.0;
	if (value > 0.0) return  1.0;
	return 0.0;
}

// угол между векторами
EDTERRAIN_API double AngleTo(const osg::Vec2d& v0, const osg::Vec2d& v1);
// площадь тр-ка
EDTERRAIN_API double TriArea(const osg::Vec2d& v0, const osg::Vec2d& v1, const osg::Vec2d& v2);
// площадь полигона
EDTERRAIN_API double TriArea(const ed::vector<osg::Vec3d>& verts);

//================================================================================
// addToVertices
//    Добавляет вершину newVertex в массив вершин vertices и возвращает её индекс
//    в этом массиве. Если расстояние от newVertex до какой-то из уже добавленных
//    вершин меньше threshold, то новая вершины не добавляется, а просто 
//    возвращается индекс уже существующей.
//================================================================================
EDTERRAIN_API size_t addToVertices(const osg::Vec3d &newVertex, ed::vector<osg::Vec3d> &vertices, double threshold);

//================================================================================
// signedPolygonAreaOXZ
//    Площадь полигона (работает и для невыпуклых), образуемого проекциями точек 
//    на плоскость OXZ. Координата y игнорирется. Площадь возвращается со знаком
//    минус если точки завёрнуты против часовой стрелки. В Indexed версию функции
//    передаётся массив всех вершин и индексы вершин, образующих полигон.
//================================================================================
EDTERRAIN_API double signedPolygonAreaOXZ(const ed::vector<osg::Vec3d> &vertices);
EDTERRAIN_API double signedPolygonAreaOXZIndexed(const ed::vector<osg::Vec3d> &vertices, const ed::vector<int> &indicies);

//================================================================================
// polygonAreaOXZ
//    Площадь полигона (работает и для невыпуклых), образуемого проекциями точек
//    на плоскость OXZ. Координата y игнорирется. Всегда больше нуля.
//================================================================================
//В OSG. Виталик
//Перенесено как ed::polygonAreaOXZ
inline double polygonAreaOXZ(const ed::vector<osg::Vec3d> &vertices)
{
	return abs(::signedPolygonAreaOXZ(vertices));
}

//В OSG. Виталик
//Перенесено как ed::polygonAreaOXZIndexed
inline double polygonAreaOXZIndexed(const ed::vector<osg::Vec3d> &vertices, const ed::vector<int> &indicies)
{
	return abs(::signedPolygonAreaOXZIndexed(vertices, indicies));
}

//================================================================================
// clockwise
//    Возвращает true если проекции точек на плоскость OXZ завёрнуты 
//    по часовой стрелке.
//================================================================================
//В OSG. Виталик
//Перенесено как ed::clockwise
inline bool clockwise(const ed::vector<osg::Vec3d> &vertices)
{
	return ::signedPolygonAreaOXZ(vertices) > 0.0;
}

//В OSG. Виталик
//Перенесено как ed::clockwiseIndexed
inline bool clockwiseIndexed(const ed::vector<osg::Vec3d> &vertices, const ed::vector<int> &indicies)
{
	return ::signedPolygonAreaOXZIndexed(vertices, indicies) > 0.0;
}
//================================================================================
// circumcircleCenterOXZ
//    Центр окружности, описанной вокруг треугольника [v0, v1, v2]
//    Координата y игнорируется. При треугольниках, близких к вырожденным в линию,
//    радиус описанной окружности стремится к бесконечности, а её центр становится
//    неопределённым поэтому всегда проверяйте результат данной функции либо не
//    запихивайте в неё "кривые" треугольники.
//================================================================================
EDTERRAIN_API osg::Vec3d circumcircleCenterOXZ(const osg::Vec3d &v0, const osg::Vec3d &v1, const osg::Vec3d &v2);

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
EDTERRAIN_API int isQuadConvex(const osg::Vec3d &a, const osg::Vec3d &b, const osg::Vec3d &c, const osg::Vec3d &d, double threshold);

//Нахождение расстояния от точки до прямой
EDTERRAIN_API double GetLengthPointToLine(
	const osg::Vec3d &v,				//Координаты точки
	const osg::Vec3d &v1,				//Первая точка линии
	const osg::Vec3d &v2,				//Вторая точка линии
	double epsilon = 0.01
	);

//Нахождение кратчайшего растояния от точки до отрезка
EDTERRAIN_API double GetPointToSegmentLength(
	const osg::Vec3d &v,				//Координаты точки
	const osg::Vec3d &v1,				//Первая точка отрезка
	const osg::Vec3d &v2,				//Вторая точка отрезка	
	osg::Vec3d &res,					//Ближайшая точка на отрезке
	double epsilon						//Допустимая погрешность
	);

//Нахождение кратчайшего растояния от точки до отрезка
EDTERRAIN_API double GetSegmentClosestPoint(
	const osg::Vec2d &v,				//Координаты точки
	const osg::Vec2d &v1,				//Первая точка отрезка
	const osg::Vec2d &v2,				//Вторая точка отрезка	
	double &param,						//Ближайшая точка на отрезке
	double epsilon						//Допустимая погрешность
	);

//Проверка пересекаются ли отрезки, координата 'y' игнорируется
EDTERRAIN_API bool IsSegmentsCross(
	const osg::Vec3d &v11,				//Первая точка 1 отрезка
	const osg::Vec3d &v12,				//Вторая точка 1 отрезка
	const osg::Vec3d &v21,				//Первая точка 2 отрезка
	const osg::Vec3d &v22,				//Вторая точка 2 отрезка
	osg::Vec3d &res,					//out: Точка пересечения
	double epsilon = 0.01				//Допустимая погрешность
	);
//Проверка пересекаются ли отрезки 2D
EDTERRAIN_API bool IsSegmentsCross(
	const osg::Vec2d &v11,				//Первая точка 1 отрезка
	const osg::Vec2d &v12,				//Вторая точка 1 отрезка
	const osg::Vec2d &v21,				//Первая точка 2 отрезка
	const osg::Vec2d &v22,				//Вторая точка 2 отрезка
	osg::Vec2d &res,					//out: Точка пересечения
	double epsilon = 0.01				//Допустимая погрешность
	);

EDTERRAIN_API bool IsSegmentsCross(
	const osg::Vec3f &v11,				//Первая точка 1 отрезка
	const osg::Vec3f &v12,				//Вторая точка 1 отрезка
	const osg::Vec3f &v21,				//Первая точка 2 отрезка
	const osg::Vec3f &v22,				//Вторая точка 2 отрезка
	osg::Vec3f &res,					//out: Точка пересечения
	float epsilon = 0.01f				//Допустимая погрешность
	);

//Проверка пересекаются ли прямые, координата 'y' игнорируется
EDTERRAIN_API bool IsLineCross(
   const osg::Vec3d &v11,				//Первая точка 1 прямой
   const osg::Vec3d &v12,				//Вторая точка 1 прямой
   const osg::Vec3d &v21,				//Первая точка 2 прямой
   const osg::Vec3d &v22,				//Вторая точка 2 прямой
   osg::Vec3d &res,						//out: Точка пересечения
   double epsilon = 0.01				//Допустимая погрешность
   );

EDTERRAIN_API bool IsLineCross(
	const osg::Vec3f &v11,				//Первая точка 1 прямой
	const osg::Vec3f &v12,				//Вторая точка 1 прямой
	const osg::Vec3f &v21,				//Первая точка 2 прямой
	const osg::Vec3f &v22,				//Вторая точка 2 прямой
	osg::Vec3f &res,					//out: Точка пересечения
	float epsilon = 0.01				//Допустимая погрешность
	);

//Проверка пересекаются ли прямые, координата 2D
EDTERRAIN_API bool IsLineCross(
   const osg::Vec2d &v11,				//Первая точка 1 прямой
   const osg::Vec2d &v12,				//Вторая точка 1 прямой
   const osg::Vec2d &v21,				//Первая точка 2 прямой
   const osg::Vec2d &v22,				//Вторая точка 2 прямой
   osg::Vec2d &res,						//out: Точка пересечения
   double epsilon = 0.01				//Допустимая погрешность
   );

//================================================================================
// triangleIntersect - проверка пересечения двух треугольников
//
// a      - вершины первого треугольника
// b      - вершины второго треугольника
// return - пересекаются ли треугольники
//================================================================================
EDTERRAIN_API bool triangleIntersect(
	const ed::vector<osg::Vec3d> &a, 
	const ed::vector<osg::Vec3d> &b,
	double epsilon = 0.01);

//================================================================================
// triangleIntersect - проверка пересечения отрезка с треугольником
//
// s1, s2		- вершины отрезка
// t1, t2, t3   - вершины треугольника
// return - пересекается ли отрезок с треугольником
//================================================================================
EDTERRAIN_API bool IsSegmentCrossTriangle(
	osg::Vec3d &s1,
	osg::Vec3d &s2,
	osg::Vec3d &t1,
	osg::Vec3d &t2,
	osg::Vec3d &t3,
	osg::Vec3d &res,
	double epsilon = 0.01
	);

EDTERRAIN_API bool IsSegmentCrossTriangle(
	osg::Vec3f &s1,
	osg::Vec3f &s2,
	osg::Vec3f &t1,
	osg::Vec3f &t2,
	osg::Vec3f &t3,
	osg::Vec3f &res,
	float epsilon = 0.01
	);

//Проверяет находится ли точчка внутри треугольника, 
//возвращает барицентрические координаты
EDTERRAIN_API int IsPointIntoTriangle(
	const osg::Vec3d &point,			//точка
	const osg::Vec3d &p1,				//1 вершина треугольника	
	const osg::Vec3d &p2,				//2 вершина треугольника
	const osg::Vec3d &p3,				//3 вершина треугольника
	osg::Vec3d &bariCoord				//Барицентрические координаты
	);

//Проверяет находится ли точчка внутри треугольника, 
//возвращает барицентрические координаты
EDTERRAIN_API int IsPointIntoTriangle(
	const osg::Vec2d &point,			//точка
	const osg::Vec2d &p1,				//1 вершина треугольника
	const osg::Vec2d &p2,				//2 вершина треугольника
	const osg::Vec2d &p3,				//3 вершина треугольника
	osg::Vec3d &bariCoord				//Барицентрические координаты
	);

//Проверяет находится ли точчка внутри треугольника, 
//возвращает барицентрические координаты
EDTERRAIN_API int IsPointIntoTriangle(
	const osg::Vec3f &point,			//точка
	const osg::Vec3f &p1,				//1 вершина треугольника	
	const osg::Vec3f &p2,				//2 вершина треугольника
	const osg::Vec3f &p3,				//3 вершина треугольника
	osg::Vec3f &bariCoord				//Барицентрические координаты
	);

//умножает массив точек на матрицу
EDTERRAIN_API void CalculateTransformPoint(
	ed::vector<osg::Vec3d> &res,				//результат
	const ed::vector<osg::Vec3d> &points,		//начальная позиция
	const osg::Matrixd &matrix					//матрица
	);

/*/
//Вычисление Tangent Space
EDTERRAIN_API void CalcTangentSpace(
	osg::Vec3d verts[3],				//кординаты вершин треугольника
	osg::Vec2d uv[3],					//текстурные координаты треугольника
	osg::Vec3d& s,						//out: вектор s
	osg::Vec3d& t						//out: вектор t
	); 

//Вычисление Tangent Space
EDTERRAIN_API void CalcTangentSpace(
	osg::Vec3f verts[3],				//кординаты вершин треугольника
	osg::Vec2f uv[3],					//кординаты вершин треугольника
	osg::Vec3f& s,						//out: вектор s
	osg::Vec3f& t						//out: вектор t
	); 
/*/

//Перевод из пространства текстурных координат в 3D пространство
//u, v - базис в пространстве текстурных координат
//s, t - базис в Tangent Space
//Вектора должны быть нормированны
//m - базис в 3D
EDTERRAIN_API void BuildBasisFromTextureSpace(
	osg::Matrixd &m,					//Результирующая матрица
	const osg::Vec2d &u,				//вектор u
	const osg::Vec2d &v,				//вектор v
	const osg::Vec3d &s,				//вектор s
	const osg::Vec3d &t					//вектор t						  
	);

//Перевод из пространства 3D в пространство текстурных координат
//m - базис в 3D
//s, t - базис в Tangent Space
//u, v - базис в пространстве текстурных координат
EDTERRAIN_API void BuildBasisForTextureSpace(
	osg::Vec2d &u,						//вектор u
	osg::Vec2d &v,						//вектор v
	const osg::Matrixd &m,				//базис в 3D
	const osg::Vec3d &s,				//вектор s
	const osg::Vec3d &t					//вектор t						  
	);

// Аппроксимация полилинии 
// Алгоритм Дугласа-Пекера (Douglas-Peucker)
EDTERRAIN_API void Approximate_DouglasPeucker(
	ed::vector<osg::Vec2d>& verts, // модифицируемая линия
	double approctolerance
	);
// надо доделать (работает в 2d)
EDTERRAIN_API void Approximate_DouglasPeucker(
	ed::vector<osg::Vec3d>& verts, // модифицируемая линия
	double approctoleranceXZ, 
	double approctoleranceY
	);

#endif