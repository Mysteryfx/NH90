#ifndef QUADTREE_H
#define QUADTREE_H

#if 0 // Реализация QuadTree через GenericQuadTree, пока тестируется

#include "GenericQuadTree.h"

class osgVec3dComparator
{
public:

	inline bool operator() (const osg::Vec3d& a, const osg::Vec3d& b, double threshold) const
	{
		osg::Vec3d d = a - b;
		double squareDistance = d.x() * d.x() + d.z() * d.z();
		return squareDistance < threshold * threshold;
	}
};

typedef GenericQuadTree<osg::Vec3d, osgVec3dComparator> QuadTree;

#else

#include <assert.h>
#include <stdint.h>
#include <ed/vector.h>
#include <algorithm>
#include <osg/Vec3d>
#include <osg/BoundingBox>

namespace ed
{
	//================================================================================
	// QuadTree
	//================================================================================
	const int QuadTreeNodeMaxVertexCount = 100;
	const int QuadTreeMaxDepth = 100;

	class QuadTree
	{
			struct Node
			{
				const ed::vector<osg::Vec3d> &vertices; // Вершины, хранящеися в дереве
				ed::vector<int32_t> vertexIndices;      // Индексы вершин, хранящеися в листе
				Node *childs[4];                         // Чайлды
				osg::Vec3d center;                       // Центр ноды
				double size;                             // Размер ноды. Необходим, чтоб рассчитывать положения центров чайлдов
			
				Node(const ed::vector<osg::Vec3d> &vertices, const osg::Vec3d &center, double size) : vertices(vertices), center(center), size(size)
				{
					childs[0] = childs[1] = childs[2] = childs[3] = NULL;
					vertexIndices.reserve(QuadTreeNodeMaxVertexCount);
				}

				//================================================================================
				// ~Node
				//    Не забываем чистить память
				//================================================================================
				~Node()
				{
					if (childs[0] != NULL) // Чайлдов либо нет ни одного, либо есть все
					{
						delete childs[0];
						delete childs[1];
						delete childs[2];
						delete childs[3];
					}
				}

				//================================================================================
				// find
				//   Поиск вершины в дереве с погрешность threshold
				//================================================================================
				int32_t find(const osg::Vec3d &vertex, double threshold2d, double threshold3d) const
				{
					if (childs[0] == NULL) // Чайлдов либо нет ни одного, либо есть все
					{
						// Если это лист, то просто сравниваем со всеми точками
						double threshold2d2 = threshold2d * threshold2d;
						double threshold3d2 = threshold3d * threshold3d;

						for (uint32_t i = 0; i < vertexIndices.size(); i++)
						{
							osg::Vec3d d = vertices[vertexIndices[i]] - vertex;
							// test 3d
							if ( d.length2() < threshold3d2)
								return vertexIndices[i];
							// test 2d
							// Расстояние по y игнорируем
							if (((d.x() * d.x()) + (d.z() * d.z())) < threshold2d2)
								return vertexIndices[i];
						}
					}
					else
					{
						// Либо ищем эту точку в чайлдах
						osg::Vec3d delta = vertex - center;

						// Когда вершина находится близко к секущей плоскости, то необходимо проверить
						// в с соседних чайлдах т.к. там тоже могут быть вершины близкие к секущей плоскости
						// и расстояние между ними будет меньше delta
						bool bothX = abs(delta.x()) < threshold2d || abs(delta.x()) < threshold3d;
						bool bothZ = abs(delta.z()) < threshold2d || abs(delta.z()) < threshold3d;

						int32_t result[4] = {-1, -1, -1, -1};

						if (bothX || delta.x() >= 0.0)
						{
							if (bothZ || delta.z() >= 0.0)
								result[0] = childs[0]->find(vertex, threshold2d, threshold3d);

							if (bothZ || delta.z() < 0.0)
								result[3] = childs[3]->find(vertex, threshold2d, threshold3d);
						}

						if (bothX || delta.x() < 0.0)
						{
							if (bothZ || delta.z() >= 0.0)
								result[1] = childs[1]->find(vertex, threshold2d, threshold3d);

							if (bothZ || delta.z() < 0.0)
								result[2] = childs[2]->find(vertex, threshold2d, threshold3d);
						}

						for (int32_t i = 0; i < 4; i++)
							if (result[i] != -1) return result[i];
					}

					return -1;
				}

				//================================================================================
				// insert
				//    Вставление вершины в дерево
				//================================================================================
				void insert(int32_t index, int32_t depth)
				{
					if (childs[0] == NULL) // Чайлдов либо нет ни одного, либо есть все
					{
						// Если это лист, то вставляем в него вершину
						vertexIndices.push_back(index);

						// Если в листе собралось слишком много вершин, то подразбиваем его
						if (vertexIndices.size() == QuadTreeNodeMaxVertexCount && depth < QuadTreeMaxDepth)
						{
							double hsize = size / 2.0;

							childs[0] = new Node(vertices, osg::Vec3d(center.x() + hsize, center.y(), center.z() + hsize), hsize);
							childs[1] = new Node(vertices, osg::Vec3d(center.x() - hsize, center.y(), center.z() + hsize), hsize);
							childs[2] = new Node(vertices, osg::Vec3d(center.x() - hsize, center.y(), center.z() - hsize), hsize);
							childs[3] = new Node(vertices, osg::Vec3d(center.x() + hsize, center.y(), center.z() - hsize), hsize);

							for (uint32_t i = 0; i < QuadTreeNodeMaxVertexCount; i++)
								insert(vertexIndices[i], depth + 1);

							vertexIndices.clear();
						}
					}
					else
					{
						// Спускаемся к листьям
						osg::Vec3d delta = vertices[index] - center;
					
						if (delta.x() >= 0.0)
							if (delta.z() >= 0.0)
								childs[0]->insert(index, depth + 1);
							else
								childs[3]->insert(index, depth + 1);
						else
							if (delta.z() >= 0.0)
								childs[1]->insert(index, depth + 1);
							else
								childs[2]->insert(index, depth + 1);
					}
				}

				//================================================================================
				// remove
				//================================================================================
				void remove(int32_t vertexIndex)
				{
					if (childs[0] == NULL) // Чайлдов либо нет ни одного, либо есть все
					{
						// Удаляем вершину
						ed::vector<int32_t>::iterator it = std::find(vertexIndices.begin(), vertexIndices.end(), vertexIndex);
						if (it != vertexIndices.end())
							vertexIndices.erase(it);
					}
					else
					{
						// Спускаемся к листьям
						osg::Vec3d delta = vertices[vertexIndex] - center;
					
						if (delta.x() >= 0.0)
							if (delta.z() >= 0.0)
								childs[0]->remove(vertexIndex);
							else
								childs[3]->remove(vertexIndex);
						else
							if (delta.z() >= 0.0)
								childs[1]->remove(vertexIndex);
							else
								childs[2]->remove(vertexIndex);
					}
				}

				//================================================================================
				// rectangleIntersect
				//    Пересекаются ли прямоугольники
				//================================================================================
				bool rectangleIntersect(const osg::Vec3d &minA, const osg::Vec3d &maxA, const osg::Vec3d &minB, const osg::Vec3d &maxB)
				{
					return std::max(minA.x(), minB.x()) <= std::min(maxA.x(), maxB.x()) &&
						   std::max(minA.z(), minB.z()) <= std::min(maxA.z(), maxB.z());
				}

				//================================================================================
				// getVerticesInRectangle
				//================================================================================
				void getVerticesInRectangle(const osg::Vec3d &min, const osg::Vec3d &max, ed::vector<int32_t> &inside)
				{
					osg::Vec3d nodeMin(center.x() - size, 0.0, center.z() - size);
					osg::Vec3d nodeMax(center.x() + size, 0.0, center.z() + size);

					if (rectangleIntersect(min, max, nodeMin, nodeMax))
					{
						for (uint32_t i = 0; i < vertexIndices.size(); i++)
						{
							const osg::Vec3d &vertex = vertices[ vertexIndices[i] ];

							if (vertex.x() >= min.x() && vertex.z() >= min.z() && vertex.x() <= max.x() && vertex.z() <= max.z())
								inside.push_back(vertexIndices[i]);
						}

						if (childs[0] != NULL)
						{
							childs[0]->getVerticesInRectangle(min, max, inside);
							childs[1]->getVerticesInRectangle(min, max, inside);
							childs[2]->getVerticesInRectangle(min, max, inside);
							childs[3]->getVerticesInRectangle(min, max, inside);
						}
					}
				}
			};

			Node root;                         // Корень дерева
			ed::vector<osg::Vec3d> &vertices; // Вершины, хранящеися в дереве
			double threshold2d;               // Расстояние ближе которого мержить вершины в 2d
			double threshold3d;               // Расстояние ближе которого мержить вершины в 3d

			//================================================================================
			// init
			//================================================================================
			void init()
			{
				if (this->vertices.size() > 0)
				{
					for (uint32_t i = 0; i < this->vertices.size(); i++)
						this->root.insert(i, 0);
				}
			}

		public:

			QuadTree(ed::vector<osg::Vec3d> &vertices, const osg::Vec3d &center, double size, double threshold2d = 0.1, double threshold3d = 0) : vertices(vertices), root(vertices, center, size), threshold2d(threshold2d), threshold3d(threshold3d)
			{
				init();
			}

			QuadTree(ed::vector<osg::Vec3d> &vertices, const osg::BoundingBox &bbox, double threshold2d = 0.1, double threshold3d = 0) : vertices(vertices), root(vertices, bbox.center(), bbox.radius()+0.1), threshold2d(threshold2d), threshold3d(threshold3d)
			{
				init();
			}
			void setThresholds(double threshold2d, double threshold3d)
			{
				this->threshold2d = threshold2d; 
				this->threshold3d = threshold3d;
			}
			//================================================================================
			// getVertices
			//    Отдаёт вектор в котором лежает вершины квадродерева
			//================================================================================
			const ed::vector<osg::Vec3d>& getVertices() const
			{
				return this->vertices;
			}

			//================================================================================
			// Найти в дереве вершину vertex. Возвращает -1 если такой вершины в дереве нет
			//================================================================================
			int32_t find(const osg::Vec3d &vertex, double threshold2d, double threshold3d=0) const
			{
				return root.find(vertex, threshold2d, threshold3d);
			}

			int32_t find(const osg::Vec3d &vertex) const
			{
				return find(vertex, this->threshold2d, this->threshold3d);
			}

			//================================================================================
			// Вставить вершину в дерево. Если вершина находится за пределами баундинг бокса
			// дерева, то она не вставляется. Если такая вершина в дереве уже есть, то 
			// возвращается индекс уже существующей.
			//================================================================================
			int32_t insert(const osg::Vec3d &vertex, double threshold2d, double threshold3d=0)
			{
				if (vertex.x() < root.center.x() - root.size || vertex.x() > root.center.x() + root.size ||
					vertex.z() < root.center.z() - root.size || vertex.z() > root.center.z() + root.size)
					return -1;

				double threshold = std::max(threshold2d, threshold3d);
				int32_t index = threshold > 0.0 ? root.find(vertex, threshold2d, threshold3d) : -1;

				if (index == -1)
				{
					assert(!osg::isNaN(vertex.x()) && !osg::isNaN(vertex.y()) && !osg::isNaN(vertex.z()));

					vertices.push_back(vertex);
					root.insert(vertices.size() - 1, 0);
					return vertices.size() - 1;
				}
				else
					return index;
			}

			int32_t insert(const osg::Vec3d &vertex)
			{
				return insert(vertex, this->threshold2d, this->threshold3d);
			}

			//================================================================================
			// Удаление вершины из дерева по индексу
			//================================================================================
			void remove(int32_t vertexIndex)
			{
				this->root.remove(vertexIndex);
			}

			//================================================================================
			// Вернуть индексы вершин дерева, попадающих в заданный прямоугольник
			//================================================================================
			void getVerticesInRectangle(const osg::Vec3d &min, const osg::Vec3d &max, ed::vector<int32_t> &inside)
			{
				this->root.getVerticesInRectangle(min, max, inside);
			}
	};
}

#endif // USE_GENERIC_QUAD_TREE

#endif