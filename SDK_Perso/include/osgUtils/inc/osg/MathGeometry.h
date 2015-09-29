#ifndef MATH_GEOMETRY_H
#define MATH_GEOMETRY_H

#ifndef DISABLE_OBSOLETE_OSGUTILS	// prohibit to use obsolete functions

#include <osg/Matrixd>
#include <osg/Vec2d>
#include <osg/Vec3d>

#include "ed/vector.h"

/* deprecated */// ИСПОЛЬЗОВАТЬ ed::segmentIntersection
/* deprecated *///Проверка пересекаются ли отрезки, координата 'y' игнорируется
/* deprecated */bool IsSegmentsCross(
/* deprecated */    const osg::Vec3d& v11,				//Первая точка 1 отрезка
/* deprecated */    const osg::Vec3d& v12,				//Вторая точка 1 отрезка
/* deprecated */    const osg::Vec3d& v21,				//Первая точка 2 отрезка
/* deprecated */    const osg::Vec3d& v22,				//Вторая точка 2 отрезка
/* deprecated */    osg::Vec3d& res,					//out: Точка пересечения
/* deprecated */    double epsilon = 0.01				//Допустимая погрешность
/* deprecated */);
/* deprecated */
/* deprecated */// ИСПОЛЬЗОВАТЬ ed::lineIntersection
/* deprecated */// Проверка пересекаются ли прямые, координата 'y' игнорируется
/* deprecated */bool IsLineCross(
/* deprecated */    const osg::Vec3d& v11,				//Первая точка 1 прямой
/* deprecated */    const osg::Vec3d& v12,				//Вторая точка 1 прямой
/* deprecated */    const osg::Vec3d& v21,				//Первая точка 2 прямой
/* deprecated */    const osg::Vec3d& v22,				//Вторая точка 2 прямой
/* deprecated */    osg::Vec3d& res,						//out: Точка пересечения
/* deprecated */    double epsilon = 0.01				//Допустимая погрешность
/* deprecated */);
/* deprecated */
/* deprecated */// ИСПОЛЬЗОВАТЬ ed::isPointInsideTriangle
/* deprecated */// Проверяет находится ли точчка внутри треугольника,
/* deprecated */// возвращает барицентрические координаты
/* deprecated */int IsPointIntoTriangle(
/* deprecated */    const osg::Vec3d& point,			//точка
/* deprecated */    const osg::Vec3d& p1,				//1 вершина треугольника
/* deprecated */    const osg::Vec3d& p2,				//2 вершина треугольника
/* deprecated */    const osg::Vec3d& p3,				//3 вершина треугольника
/* deprecated */    osg::Vec3d& bariCoord				//Барицентрические координаты
/* deprecated */);
/* deprecated */
/* deprecated */// ИСПОЛЬЗОВАТЬ ed::isPointInsideTriangle
/* deprecated */// Проверяет находится ли точчка внутри треугольника,
/* deprecated */// возвращает барицентрические координаты
/* deprecated */int IsPointIntoTriangle(
/* deprecated */    const osg::Vec2f& point,			//точка
/* deprecated */    const osg::Vec2f& p1,				//1 вершина треугольника
/* deprecated */    const osg::Vec2f& p2,				//2 вершина треугольника
/* deprecated */    const osg::Vec2f& p3,				//3 вершина треугольника
/* deprecated */    osg::Vec3f& bariCoord				//Барицентрические координаты
/* deprecated */);

//умножает массив точек на матрицу
void CalculateTransformPoint(
    ed::vector<osg::Vec3d> &res,				//результат
    const ed::vector<osg::Vec3d> &points,		//начальная позиция
    const osg::Matrixd& matrix					//матрица
);

#endif //DISABLE_OBSOLETE_OSGUTILS
#endif // MATH_GEOMETRY_H