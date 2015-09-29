#ifndef Geometry_H
#define Geometry_H

#include "Resourcer\Resource.h"
#include "GraphMath/box.h"
#include "DynamicParams.h"
#include "RenderObject.h"

namespace Graphics
{

class CustomGeometry;

// Интерфейс для элементов геометрии
class Geometry : public Resource
{
public:
	Geometry()	{rtype = rtGeometry;}
	
	// габариты
	virtual const Box& GetBoundingBox() = 0;
    virtual float GetRadius() = 0;
	virtual const Vector3& GetCenter() = 0;

	// статистика
	virtual unsigned GetPolygonCount() = 0;

	// отрисовка
	virtual void DrawGeometry(RenderObject *robject) = 0;
	
	// детальная проверка столкновений
	// пересечение с отрезком [a,b] при параметрах params
	// может вернуть параметр t на отрезке и нормаль n в точке пересечения
	virtual bool IntersectSegment(const Vector3& a, const Vector3& b, const DParamList& params, 
		float *t = 0, Vector3 *n = 0) = 0;

	// сортировка и оптимизация
	virtual GeometryKey GetKey() = 0;

	// редактирование
	virtual CustomGeometry *QueryCustomGeometry(){return 0;}

	// загрузка данных
	virtual void LoadData(void *data, unsigned maxsize) = 0;
};

}

#endif // Geometry_H
