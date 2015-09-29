#ifndef CUT_IN_COMMON_H
#define CUT_IN_COMMON_H

#include "osg/Quadtree.h"
#include "osg/SegmentQuadTree.h"
#include "offshore/scene.h"
#include "offshore/PolygonQuadTree.h"
#include "ed/set.h"


namespace offshore {

//===================================================================================
// SegmentSpline 
//    Ломаная, состоящая из отрезков.
//===================================================================================
struct SegmentSpline
{
	ed::list<ed::Segment> segments; // отрезки

	//===============================================================================
	// Polyline - построить ломаную из оффшор сплайна
	//===============================================================================
	OFFSHORE_API SegmentSpline(const offshore::Spline &offshoreSpline)
	{
		for (uint32_t i = 1; i < offshoreSpline.verts.size(); i++)
			segments.push_back(ed::Segment(offshoreSpline.verts[i - 1], offshoreSpline.verts[i]));
	}

	//===============================================================================
	// convertToOffshore
	//    Конвертация обратно в offshore сплайн
	//===============================================================================
	OFFSHORE_API void convertToOffshore(offshore::Spline &spline)
	{
		spline.tangents_forward.clear();
		spline.tangents_backward.clear();

		spline.tangents_forward.resize(segments.size() + 1, osg::Vec3d());
		spline.tangents_backward.resize(segments.size() + 1, osg::Vec3d());

		spline.verts.clear();
		spline.verts.reserve(segments.size() + 1);

		ed::list<ed::Segment>::iterator it = segments.begin();
		spline.verts.push_back(it->a);

		for (; it != segments.end(); ++it)
			spline.verts.push_back(it->b);
	}

	//===============================================================================
	// divide
	//    Разбить в ломаной пересекаемые отрезки на отдельные непересекаемые отрезки
	//===============================================================================
	OFFSHORE_API void divide()
	{
		for (ed::list<ed::Segment>::iterator it = segments.begin(); it != segments.end();)
		{
			ed::Segment &segment = *it; ++it;

			if (segment.intersections.size() > 0)
			{
				osg::Vec3d a = segment.a, b = segment.b;

				// Первая точка пересечения
				ed::set<double>::iterator value = segment.intersections.begin();

				// Заменяем исходный отрезок на первый в разбиении
				segment.b = ed::lerp(a, b, *value);

				// Идём по всем точкам пересечения и добавляем новые отрезки
				double prev = *value;
				for (++value; value != segment.intersections.end(); ++value)
				{
					segments.insert(it, ed::Segment(ed::lerp(a, b, prev), ed::lerp(a, b, *value)));
					prev = *value;
				}

				// Добавляем последний отрезок
				segments.insert(it, ed::Segment(ed::lerp(a, b, prev), b));

				// Пересечений больше нету
				segment.intersections.clear();
			}
		}
	}
};


//================================================================================
// buildSegmentQuadTree
//    Строит квадродерево рёбер меша
//================================================================================
OFFSHORE_API void buildSegmentQuadTree(ed::SegmentQuadTree &segQuadTree, offshore::Mesh &mesh);

//================================================================================
// buildVertexPolygonAdjacency
//    Смежность вершин и полигонов
//================================================================================
OFFSHORE_API ed::vector<ed::vector<int32_t> > buildVertexPolygonAdjacency(offshore::Mesh &mesh);

//================================================================================
// removePolygons
//    Удаляет из меша mesh полигоны
//================================================================================
OFFSHORE_API void removePolygons(offshore::Mesh& mesh, const ed::vector<uint8_t>& removePolygon);
OFFSHORE_API void removePolygons(offshore::uvSet& uvSet, const ed::vector<uint8_t>& removePolygon);

//================================================================================
// buildPolygonQuadTree
//    Строит квадродерево полигонов меша
//================================================================================
OFFSHORE_API void buildPolygonQuadTree(offshore::PolygonQuadTree &polyQuadTree, const ed::vector<offshore::Polygon> &polygons);

//================================================================================
// projectPoint
//    Спроецировать точку на меш
//    vertex       - вершина, которую проецируем
//    polyQuadTree - квад-дерево, в котором лежат полигоны меша
//================================================================================
OFFSHORE_API osg::Vec3d projectPoint(osg::Vec3d &vertex, offshore::PolygonQuadTree &polyQuadTree);

//================================================================================
// getMeshCount
//    Число мешей в сцене. scene.meshes.size() не катит т.к. там могут валяться
//    меши для которых не существует нод в дереве.
//================================================================================
OFFSHORE_API uint32_t getMeshCount(offshore::Scene &scene);

//===================================================================================
// computeUV
//    Посчитать uv для точки с индексом vertexIndex для маппинга полигона с индексом
//    polygonIndex. Считаем что на все полигоны наложен планарный маппинг в плоскости
//    OXZ.
//
//    vertexIndex  - индекс вершины
//    polygonIndex - индекс полигона
//    vertices     - массив вершин
//    polygons     - массив полигонов
//    uvSet        - uvSet для которого проводятся вычисления
//===================================================================================
OFFSHORE_API
osg::Vec2d computeUV(const osg::Vec3d &p, int32_t polygonIndex, const ed::vector<osg::Vec3d> &vertices, 
	const ed::vector<offshore::Polygon> &polygons, offshore::uvSet &uvSet);

//================================================================================
// cutInSpline
//    Врезка сплайнов в меш. Сохраняет внешние рёбра меша. Проецирует сплайн на меш.
//    Удаляет слишком близкие вершины.
//
//    mesh                 - меш в который врезаем
//    splines              - сплайны которые врезаем
//    removeVertexDistance - расстояние для удаления слишком близких вершин
//    vertexWeldDistance   - расстояние для велда вершин
//    errors               - ошибки
//================================================================================
 


OFFSHORE_API void roughCutInSpline(
	offshore::Mesh &mesh, 
	ed::vector<offshore::Spline> &splines, 
	double removeVertexDistance, 
	std::function<void(int spline)>* funcProgress=nullptr);
OFFSHORE_API void roughCutInSpline(
	offshore::Mesh &mesh, ed::vector<SegmentSpline> &splines,
	ed::SegmentQuadTree*& segQuadTree,
	ed::vector<uint8_t>& removePolygon,
	double removeVertexDistance, 
	std::function<void(int spline)>* funcProgress=nullptr);

// Найти треугольники меша пересекающиеся со сплайнами
OFFSHORE_API void findMeshPoyUnderSegments(
	offshore::Mesh &mesh, 
	const ed::vector<ed::Segment>& segments,
	ed::set<uint32_t>& selectedPolygons,
	double distance
	);



} // namespace offshore

#endif
