#ifndef BSP_TREE_H
#define BSP_TREE_H

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lGeometry.h"
#include "landscape4/lDataFile.h"

namespace landscape4
{

	struct BSPTree
	{
		struct Node
		{
			// Индексы вершин ребра, которое делит эту ноду
			uint16_t v1, v2;

			// Индексы самой высокой и самой низкой верщины
			uint16_t maxHeight, minHeight;

			// Индексы нод детей
			uint16_t leftChild, rightChild;

			// Индекс фэйса для листов
			uint16_t faceIndex;

			Node() :
				faceIndex(-1), maxHeight(-1), minHeight(-1), v1(-1), v2(-1), leftChild(-1), rightChild(-1) {}

			Node(uint16_t faceIndex, uint16_t maxHeight, uint16_t minHeight) : 
				faceIndex(faceIndex), maxHeight(maxHeight), minHeight(minHeight), v1(-1), v2(-1), leftChild(-1), rightChild(-1) {}
		};

		io::mmfvector<Node> nodes;
	};

	struct BSPTree2
	{
		struct Node
		{
			union
			{
				uint32_t leafPolygon;  //Номер полигона для листового узла
				struct
				{
					uint16_t v1;
					uint16_t v2;
				} divider; //Ребро, образовавшее узел для нелистовых узлов
			} otherData;
			uint16_t minHeightVertex; //Номера вершин для извлечения высот
			uint16_t maxHeightVertex;
			int32_t rightChild; // Индекс узла-корня правого поддерева, если > 0, количество полигонов в листовом узле, если<=0 (левое поддерево расположено в следующем после текущего узле)
			Node() : rightChild(0), minHeightVertex(0), maxHeightVertex(0) //Конструктор по умолчанию для вектора
			{
				otherData.divider.v1 = otherData.divider.v2 = 0;
			}  
			Node(int32_t polygonPosition, int32_t count) : rightChild(-count), minHeightVertex(0), maxHeightVertex(0)  //Для листовых узлов
			{
				otherData.leafPolygon = polygonPosition;
			}
			Node(int32_t rightPolygon, uint16_t v1, uint16_t v2) : rightChild(rightPolygon), minHeightVertex(0), maxHeightVertex(0) //Для нелистовых узлов 
			{
				otherData.divider.v1 = v1;
				otherData.divider.v2 = v2;
			}
		};

		io::mmfvector<Node> nodes;
		io::mmfvector<uint32_t> polygons;
	};
}

namespace landscape4
{
	namespace BSPIntersection
	{
		/*
			Проверяет, находится ли точка спереди ребра (оптимизированная версия, ребро передается двумя точками)
		*/
		inline double getPointPosition(const osg::Vec3f& p1, const osg::Vec3f& p2, const osg::Vec3d& point)
		{
			return (p2.x()-p1.x())*(point.z()-p1.z())-(point.x()-p1.x())*(p2.z()-p1.z());
		}
	}
}
//#include "inl/BSPTree.inl"

#endif
