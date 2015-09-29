#ifndef MESHUTILS_H
#define MESHUTILS_H

#include <osg/Vec3d>
#include "offshore/scene.h"
#include "osg/RectQuadtree.h"

namespace offshore
{

/**
 * Дерево с мешами, нужно для ускорения проверки пересечений
 */
struct MeshesTree
{
public:	
	//Детализированные ноды или нет
	OFFSHORE_API ~MeshesTree();

	/**
	 * Зарезервировать пространство
	 */
	OFFSHORE_API void reserve(size_t size);

	/** 
	 * Добавить меш в дерево
	 * @return индекс ноды меша в дереве
	 */
	OFFSHORE_API int insert(const offshore::Mesh& mesh);

	/**
	 * Собрать меши, попадающие в квадрат
	 * @param rect квадрат
	 * @param selectedMeshes индексы мешей, попадающих в квадрат
	 */
	OFFSHORE_API void select(const osg::Rect& rect, ed::vector<int>& selectedMeshes);

	/**
	 * Пересечь меш с мешами внутри дерева
	 */
	OFFSHORE_API bool intersect(const offshore::Mesh& mesh, double distance = 0.1);

	/**
	 * Очистить дерево
	 */
	OFFSHORE_API void clear();

	/**
	 * Пусто ли дерево
	 */
	bool empty() {return this->meshes.empty();}

	/**
	 * Вставить ждущие ноды
	 */
	OFFSHORE_API void insertAwaitingNodes();

// На кой к внутреним структурам доступ?
//protected:
public:
	/**
	 * Нода дерева с информацией о меше
	 */
	struct MeshTreeNode
	{
		//Меш
		offshore::Mesh mesh;

		//Открытые рёбра меша
		ed::vector<offshore::Edge32> edges;

		//Ограничивающий квадрат меша
		osg::Rect rect;

		//Дерево открытых рёбер меша
		ed::RectQuadtree<int>* edgesTree;

		//Дерево полигонов меша
		ed::RectQuadtree<int>* polygonsTree;

		OFFSHORE_API MeshTreeNode() {};
		OFFSHORE_API MeshTreeNode(const offshore::Mesh& mesh);

		/**
		 * Собрать открытые рёбра, которые попадают в квадрат
		 * @param selectRect квадрат, в котором нужно выбрать рёбра
		 * @param selectedOpenEdges индексы рёбер, находящихся в заданном квадрате
		 */
		OFFSHORE_API void selectOpenEdges(const osg::Rect& selectRect, ed::vector<int>& selectedOpenEdges) const;

		/**
		 * Собрать полигоны, которые попадают в квадрат
		 * @param selectRect квадрат, в котором нужно выбрать полигоны
		 * @param selectedPolygons индексы полигонов, находящихся в заданном квадрате
		 */
		OFFSHORE_API void selectPolygons(const osg::Rect& selectRect, ed::vector<int>& selectedPolygons) const;

		/**
		 * Пересечь ноду с другой нодой
		 * @param node нода, с которой пересекаем
		 * @param edge ребро меша данной ноды, которое участвует в пересечении
		 * @param nodeEdge ребро меша другой ноды, которое участвует в пересечении
		 * @param inside находится ли один меш внутри другого
		 * @param point точка пересечения мешей
		 * @param distance дистанция, при которой считается, что меши пересекаются
		 * @return true если найдена точка пересечения мешей
		 */
		OFFSHORE_API bool intersect(const MeshTreeNode& node, double distance = 0.1) const;
		OFFSHORE_API bool intersect(const MeshTreeNode& node, offshore::Edge32& edge, offshore::Edge32& nodeEdge, bool& inside, osg::Vec3d& point, double distance = 0.1) const;
	
		/**
		 * Лежит ли точка внутри меша ноды
		 * @param point точка
		 * @param distance дистанция, при которой считается, что меши пересекаются
		 */
		OFFSHORE_API bool pointInsideMesh(const osg::Vec3d& point, double distance = 0.1) const;

		/**
		 * Удалить деревья внутри ноды
		 */
		OFFSHORE_API void deleteTrees();
        
        /**
         * Вставить ждущие ноды
         */
        OFFSHORE_API void insertAwaitingNodes();
	};
	//Меши
	ed::vector<MeshTreeNode> meshes;
	ed::RectQuadtree<int> meshesTree;

	/**
	 * Пересечь ноду с мешами внутри дерева
	 */
	OFFSHORE_API bool intersect(const MeshTreeNode& node, double distance = 0.1);

};


//================================================================================
// isPointInsideMesh
//    Находится ли точка внутри меша. 2D проверка, координата y игнорируется
//
//    point       - точка.
//    mesh        - меш.
//    outPolygon  - номер полигона меша в который попала точка, -1 если не попала.
//    outFace     - номер фэйса полигона в который попала точка, -1 если не попала.
//    barycentric - барицентрические координаты точки в этом фэйсе.
//    threshold   - погрешность. Если поставить много, то будут захватываться точки
//                  которые на самом деле в этот полигон не попадают и наоборот.
//================================================================================
OFFSHORE_API bool isPointInsideMesh(const osg::Vec3d& point, const offshore::Mesh& mesh, int& outPolygon, 
	                   int& outFace, osg::Vec3d& barycentric, double threshold);

/**
 * Пересекаются ли меши
 * @param first первый меш
 * @param second второй меш
 * @param threshold погрешность
 */
OFFSHORE_API bool meshesIntersects(const offshore::Mesh& first, const offshore::Mesh& second, double threshold = 0.001);

/**
 * Пересекаются ли меши
 * @param first первый меш
 * @param firstTransform трансформ первого меша
 * @param second второй меш
 * @param secondTransform трансформ второго меша
 * @param threshold погрешность
 */
OFFSHORE_API bool meshesIntersects(const offshore::Mesh& first, const osg::Matrixd& firstTransform, const offshore::Mesh& second, const osg::Matrixd& secondTransform, double threshold = 0.001);


//================================================================================
// weldAndSnap
//    meshes         - входной меш
//    squareSize     - размер квадрата
//    snapDistance   - расстояние для снапа
//    weldDistance   - расстояние для велда
//    errors         - массив для ошибок
//    squareinorigin - геометрия в начале координат т.е. отцентрирована в (0,0)
//================================================================================
OFFSHORE_API void weldAndSnap(
	ed::vector<offshore::Mesh*> &meshes, 
	double squareSize, double snapDistance, double weldDistance,
	ed::vector<offshore::Error> &errors, 
	bool squareinorigin=false,				// геометрия в начале координат
	bool bRemoveIsolatedVerts=true
	);

//================================================================================
// weldAndSnap
//    splines        - входные сплайны
//    squareSize     - размер квадрата
//    snapDistance   - расстояние для снапа
//    weldDistance   - расстояние для велда
//    errors         - массив для ошибок
//    squareinorigin - геометрия в начале координат т.е. отцентрирована в (0,0)
//================================================================================
OFFSHORE_API void weldAndSnap(
	ed::vector<offshore::Spline*> &splines, 
	double squareSize, double snapDistance, double weldDistance,
	ed::vector<offshore::Error> &errors, 
	bool squareinorigin=false, 
	bool bRemoveIsolatedVerts=true
	);


}

#endif