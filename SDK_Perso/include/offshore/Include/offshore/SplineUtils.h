#ifndef __SPLINE_UTILS__
#define __SPLINE_UTILS__

#include "offshore/scene.h"
//#include "landscape4/lRoadsTypes.h"
//Работа с единичным сплайном

namespace offshore
{
	/**
	 * Заменить конец сплайна
	 * @param spline сплайн
	 * @param a новый конец сплайна
	 * @param b следующая за концом точка
	 * @param start true - заменить начало, false - заменить конец
	 * @param notifications уведомления
	 * @param mindistance минимальная дистанцифя между тчокой b и следующей точкой сплайна
	 */
	OFFSHORE_API void changeTail(offshore::Spline& spline, const osg::Vec3d& a, const osg::Vec3d& b, bool start, ed::vector<offshore::Notification>& notifications, double minDistance = 0.0);

	/**
	 * Обрезать конец сплайна
	 * @param spline сплайн
	 * @param vertex новый конец сплайна
	 * @param start true - заменить начало, false - заменить конец
	 * @param notifications уведомления
	 * @param mindistance минимальная дистанцифя между тчокой b и следующей точкой сплайна
	 */
	OFFSHORE_API void cutTail(offshore::Spline& spline, const osg::Vec3d& vertex, bool start, ed::vector<offshore::Notification>& notifications, double minDistance = 0.0);

	/**
	 * Спроецировать точки на плоскость XZ и проверить, есть ли среди проекций точек массива проекция точки
	 * @param vertices массив точек
	 * @param vertex точка
	 * @param delta абсолютная погрешность вычисления длины
	 */
	OFFSHORE_API bool contains2d(const ed::vector<osg::Vec3d>& vertices, const osg::Vec3d& vertex, double delta = 0.1);

	/**
	 * Пересекает ли сплайн сам себя
	 * @param spline сплайн
	 * @param points точки самопересечения
	 * @param delta абсолютная пошгрешность вычисления длины
	 */
	OFFSHORE_API bool selfIntersects(const offshore::Spline& spline, ed::vector<osg::Vec3d>& points, double delta = 0.1);

	/**
	 * Получить номер сегмента в сплайне, по расстоянию от начала
	 * Сегменты нумеруются с нуля
	 * @param distances расстояния от начала сплайна до точек сплайна
	 * @param distance расстояние от начала
	 * @param delta абсолютная погрешность вычисления расстояния
	 * @return возвращает номер сегмента, если расстояние меньше минимального - -1, больше максимального - максимальный сегмент + 1
	 */
	OFFSHORE_API int32_t getSegmentByDistance(const ed::vector<double>& distances, double distance, double delta = 0.01);

	/**
	 * Получить расстояния от начала сплайна до точек сплайна
	 * Расстояния считаются в плоскости XZ
	 * @param spline сплайн
	 * @param distances полученные расстояния
	 */
	OFFSHORE_API void getDistances2d(const offshore::Spline& spline, ed::vector<double>& distances);

	/**
	 * Сгладить сплайн по высоте
	 * @param spline сплайн
	 * @param delta расстояние между опорными точками для сглаживания
	 */
	OFFSHORE_API void smoothSplineByHeight(offshore::Spline& spline, double delta);

	//Работа с множеством сплайнов

	/**
	 * Упрощение сплайна (удаляются точки, расстояние между которыми меньше degradedistance)
	 * Расстояние измеряется в плоскости XZ
	 * @param scene сцена со сплайнами
	 * @param degradedistance расстояние между точками для удаленмя
	 * @param parentOperation вызывающая операция, для вывода информации в консоль
	 */
	OFFSHORE_API void splines_Degrade2d(offshore::Scene& scene, double degradedistance, const ed::string& parentOperation = "");

	/**
	 * Структура, описывающая точку стыковки сплайнов 
	 */
	struct splines_IntersectionPoint
	{
		//Точка пересечения
		osg::Vec3d pt;
		//Ноды сплайнов в сцене
		ed::vector<int> treenodes;
	};

	/**
	 * Найти точки стыковки сплайнов
	 * @param scene сцена со сплайнами
	 * @param points найденные точки стыковки
	 * @param weldDistance расстояние для велда концов сплайна
	 * @param notifications уведомления в ходе выполнения
	 * @param parentOperation вызывающая операция, для вывода информации в консоль
	 */
	OFFSHORE_API void splines_FindCrosses(offshore::Scene& scene, ed::vector<splines_IntersectionPoint>& points, double weldDistance, ed::vector<offshore::Notification>& notifications, const ed::string& parentOperation = "");

	/**
	 * Нарезать сплайны точками пересечений, с сохранением параметров объектов
	 * @param scene сцена со сплайнами
	 * @param delta погрешность измерения расстояния (Если расстояние меньше дельты -> оно равно нулю)
	 * @param parentOperation вызывающая операция, для вывода информации в консоль
	 */
	OFFSHORE_API void splines_Intersection(offshore::Scene& scene, const double delta = 0.1, const ed::string& parentOperation = "");

	/**
	 * Притянуть концы сплайнов к другим сплайнам
	 * @param scene сцена со сплайнами
	 * @param welddistance расстояние притяжки
	 * @param parentOperation вызывающая операция, для вывода информации в консоль
	 */
	OFFSHORE_API void splines_WeldTails(offshore::Scene& scene, double welddistance, const ed::string& parentOperation = "");
}

/**
 * Применить с сплайнам subdivision
 * @param scene сцена со сплайнами
 * @param roadsTypes дорожные типы
 * @param parentOperation вызывающая операция, для вывода информации в консоль
 */
//void splines_Subdivide(offshore::Scene& scene, roads4::lRoadsTypes& roadsTypes, const ed::string& parentOperation = "");

/**
 * Применить с сплайнам фильтрацию
 * @param scene сцена со сплайнами
 * @param roadsTypes типы дорог
 * @param parentOperation вызывающая операция, для вывода информации в консоль
 */
//void splines_FilterPoints(offshore::Scene& scene, roads4::lRoadsTypes& roadsTypes, const ed::string& parentOperation = "");

//Старые spline utils
/*#include "offshore/scene.h"
#include "edterrain_math.h"
#include <math.h>

struct SegmentPoints
{

private:
	static osg::Vec3d spVert;
	static bool CompareSegmentPoints (const osg::Vec3d &point1, const osg::Vec3d &point2);
public:
	SegmentPoints(){};
	SegmentPoints(osg::Vec3d &_vert)
	{
		vert = _vert;
	}
	SegmentPoints(osg::Vec3d &_vert, osg::Vec3d &point)
	{
		vert = _vert;
		points.push_back(point);
	}
	SegmentPoints(osg::Vec3d &_vert, osg::Vec3d &point1, osg::Vec3d &point2)
	{
		vert = _vert;
		points.push_back(point1);
		points.push_back(point2);
	}
	SegmentPoints(osg::Vec3d &_vert, osg::Vec3d &point1, osg::Vec3d &point2, osg::Vec3d &point3)
	{
		vert = _vert;
		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
	}
	osg::Vec3d vert;
	ed::vector<osg::Vec3d> points;
	void Sort();
};

struct PointOnSegment
{
	int segment;
	osg::Vec3d pos;
	PointOnSegment(){};
	PointOnSegment(int segment, osg::Vec3d &pos)
	{
		this->segment = segment;
		this->pos = pos;
	}
};

void IntersectSplines(offshore::Scene &scene, offshore::Scene &newScene, double epsilon = 0.1, double approximate=0.1);
void AddPointsInSpline(offshore::Spline &spline, ed::vector<PointOnSegment> &points, double epsilon = 0.1);
*/
#endif
