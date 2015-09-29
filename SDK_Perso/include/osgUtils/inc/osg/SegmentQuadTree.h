#ifndef SEGMENT_QUAD_TREE_H
#define SEGMENT_QUAD_TREE_H

#include "offshore/scene.h"

#include "IntersectionUtils.h"
#include "SegmentUtils.h"

#include <ed/set.h>

namespace ed
{
	//===================================================================================
	// Segment - структура отрезка
	//===================================================================================
	struct Segment
	{
		osg::Vec3d a, b; // Точки отрезка
		bool open;       // Является ли отрезок "открытым" в меше

		// Полигоны, содержащие отрезок
		int32_t adjacentPolygon1, adjacentPolygon2;

		// Точки на отрезке (значения от 0.0 до 1.0), в которых он пересекается, отсортированные по возрастанию
		ed::set<double> intersections;

		Segment() {}

		Segment(const osg::Vec3d &a, const osg::Vec3d &b, int32_t poly1 = -1, int32_t poly2 = -1) : a(a), b(b), open(poly2!=-1), adjacentPolygon1(poly1), adjacentPolygon2(poly2)
		{
		}

		Segment(const ed::vector<osg::Vec3d> &vertices, const std::pair<offshore::Edge32, ed::vector<int32_t> > &edge)
		{
			a = vertices[ edge.first.v1 ];
			b = vertices[ edge.first.v2 ];

			open = edge.second.size() == 1;

			adjacentPolygon1 = edge.second.size() > 0 ? edge.second[0] : -1;
			adjacentPolygon2 = edge.second.size() > 1 ? edge.second[1] : -1;
		}

		Segment(const ed::vector<osg::Vec3d> &vertices, const offshore::Edge32& edge, int32_t poly1, int32_t poly2 = -1)
		{
			a = vertices[ edge.v1 ];
			b = vertices[ edge.v2 ];

			open = poly2 == -1;

			adjacentPolygon1 = poly1;
			adjacentPolygon2 = poly2;
		}

		osg::Vec3d pointByParam(double param) const
		{
			return a * (1.0 - param) + b * (param);
		}
	};

	//===================================================================================
	// Intersection
	//===================================================================================
	struct Intersection
	{
		Segment *segment;
		// intersection position in other segment
		double paramseg1;
		// intersection position in @segment
		double paramseg2;

		Intersection(Segment &segment, double paramseg1, double paramseg2) : segment(&segment), paramseg1(paramseg1), paramseg2(paramseg2) {}
	};

	//================================================================================
	// Грубая проверка на пересечение баундинг бокса и отрезка
	//================================================================================
	inline bool hit(const Segment& segment, const osg::BoundingBox& box, double removeVertexDistance)
	{
		bool a = (segment.a.x() + removeVertexDistance) <= box.xMin() && (segment.b.x() + removeVertexDistance) <= box.xMin(); // Целиком слева
		bool b = (segment.a.x() - removeVertexDistance) >= box.xMax() && (segment.b.x() - removeVertexDistance) >= box.xMax(); // Целиком справа
		bool c = (segment.a.z() + removeVertexDistance) <= box.zMin() && (segment.b.z() + removeVertexDistance) <= box.zMin(); // Целиком снизу
		bool d = (segment.a.z() - removeVertexDistance) >= box.zMax() && (segment.b.z() - removeVertexDistance) >= box.zMax(); // Целиком сверху

		return !(a || b || c || d);
	}

	//================================================================================
	// SegmentQuadTree
	//    Квад-дерево для отрезков
	//================================================================================
	class SegmentQuadTree
	{
		ed::vector<Segment> segments;

		struct Node
		{
			osg::Vec3d center;
			double size;
			Node *childs[4];

			ed::vector<Segment> &segments;
			// Все отрезки ноды дерева разбиваем на две группы для последующего 
			// ускорения поиска пересечений
			ed::vector<int32_t> segmentIndiciesV;
			ed::vector<int32_t> segmentIndiciesH;
			osg::BoundingBox segmentsBoxV, segmentsBoxH;

			Node(const osg::Vec3d &center, double size, ed::vector<Segment> &segments) : center(center), size(size), segments(segments)
			{
				childs[0] = childs[1] = childs[2] = childs[3] = NULL;
				segmentIndiciesV.reserve(100);
				segmentIndiciesH.reserve(100);
				segmentsBoxV.set(center, center);
				segmentsBoxH.set(center, center);
			}

			// BoundingBox данной ноды в OXZ
			osg::BoundingBox getBoundingBox() const
			{
				return osg::BoundingBox(center.x() - size, 0.0, center.z() - size, center.x() + size, 0.0, center.z() + size);
			}

			// Находится ли отрезок внутри квадрата данного узла кваддерева
			bool inside(const Segment &segment, const osg::Vec3d &center, double size)
			{
				return (segment.a.x() <= center.x() + size) && (segment.a.z() <= center.z() + size)
					&& (segment.b.x() <= center.x() + size) && (segment.b.z() <= center.z() + size)
					&& (segment.a.x() >= center.x() - size) && (segment.a.z() >= center.z() - size)
					&& (segment.b.x() >= center.x() - size) && (segment.b.z() >= center.z() - size);
			}

			void insert(const Segment &segment, uint32_t depth)
			{
				double hsize = size / 2.0;

				osg::Vec3d center0 = osg::Vec3d(center.x() + hsize, 0.0, center.z() - hsize);
				osg::Vec3d center1 = osg::Vec3d(center.x() - hsize, 0.0, center.z() - hsize);
				osg::Vec3d center2 = osg::Vec3d(center.x() - hsize, 0.0, center.z() + hsize);
				osg::Vec3d center3 = osg::Vec3d(center.x() + hsize, 0.0, center.z() + hsize);

				if (depth <= 200)
				{
					if (inside(segment, center0, hsize))
					{
						if (childs[0] == NULL) childs[0] = new Node(center0, hsize, segments);
						return childs[0]->insert(segment, depth + 1);
					}
					if (inside(segment, center1, hsize))
					{
						if (childs[1] == NULL) childs[1] = new Node(center1, hsize, segments);
						return childs[1]->insert(segment, depth + 1);
					}
					if (inside(segment, center2, hsize))
					{
						if (childs[2] == NULL) childs[2] = new Node(center2, hsize, segments);
						return childs[2]->insert(segment, depth + 1);
					}
					if (inside(segment, center3, hsize))
					{
						if (childs[3] == NULL) childs[3] = new Node(center3, hsize, segments);
						return childs[3]->insert(segment, depth + 1);
					}
				}

				if ((segment.a.x() - center.x()) * (segment.b.x() - center.x()) < 0)
				{
					segmentIndiciesV.push_back(segments.size());
					segmentsBoxV.expandBy(segment.a);
					segmentsBoxV.expandBy(segment.b);
				}
				else
				{
					segmentIndiciesH.push_back(segments.size());
					segmentsBoxH.expandBy(segment.a);
					segmentsBoxH.expandBy(segment.b);
				}

				segments.push_back(segment);
			}

			void findIntersections(const Segment& segment, ed::vector<Intersection>& intersections, double removeVertexDistance, const ed::vector<int32_t>& indicies)
			{
				for (uint32_t i = 0; i < indicies.size(); i++)
				{
					Segment &seg = this->segments[indicies[i]];

					// Проверяем на пересечение с отрезками этой ноды
					double paramseg1, paramseg2;
					if (ed::segmentIntersection(segment.a, segment.b, seg.a, seg.b, paramseg1, paramseg2))
					{
						intersections.push_back(Intersection(seg, paramseg1, paramseg2));
						continue;
					}

					// Проверка по-расстоянию
					if (removeVertexDistance > 0.0)
						if (ed::distanceToSegment(seg.a, seg.b, segment.a)     < removeVertexDistance || 
							ed::distanceToSegment(seg.a, seg.b, segment.b)     < removeVertexDistance ||
							ed::distanceToSegment(segment.a, segment.b, seg.a) < removeVertexDistance ||
							ed::distanceToSegment(segment.a, segment.b, seg.b) < removeVertexDistance)
						{
							intersections.push_back(Intersection(seg, -1.0, -1.0));
						}
				}
			}

			void findIntersections(const Segment& segment, ed::vector<Intersection>& intersections, double removeVertexDistance)
			{
				// Проверяем на пересечение с отрезками данной ноды
				if (hit(segment, segmentsBoxH, removeVertexDistance))
					findIntersections(segment, intersections, removeVertexDistance, segmentIndiciesH);

				if (hit(segment, segmentsBoxV, removeVertexDistance))
					findIntersections(segment, intersections, removeVertexDistance, segmentIndiciesV);

				// Спускаемся к дочерним нодам
				if (childs[0] != NULL && hit(segment, childs[0]->getBoundingBox(), removeVertexDistance))
					childs[0]->findIntersections(segment, intersections, removeVertexDistance);

				if (childs[1] != NULL && hit(segment, childs[1]->getBoundingBox(), removeVertexDistance))
					childs[1]->findIntersections(segment, intersections, removeVertexDistance);

				if (childs[2] != NULL && hit(segment, childs[2]->getBoundingBox(), removeVertexDistance))
					childs[2]->findIntersections(segment, intersections, removeVertexDistance);

				if (childs[3] != NULL && hit(segment, childs[3]->getBoundingBox(), removeVertexDistance))
					childs[3]->findIntersections(segment, intersections, removeVertexDistance);
			}

			int32_t height()
			{
				int32_t h0 = childs[0] != NULL ? childs[0]->height() : 0;
				int32_t h1 = childs[1] != NULL ? childs[1]->height() : 0;
				int32_t h2 = childs[2] != NULL ? childs[2]->height() : 0;
				int32_t h3 = childs[3] != NULL ? childs[3]->height() : 0;

				return std::max(std::max(h0, h1), std::max(h2, h3)) + 1;
			}
		};

		Node root;

	public:

		SegmentQuadTree(const osg::BoundingBox &bbox) : root(bbox.center(), bbox.radius(), segments)
		{
			//...
		}

		void insert(const Segment &segment)
		{
			this->root.insert(segment, 1);
		}

		ed::vector<Intersection> findIntersections(const Segment &segment, double removeVertexDistance = 0.0)
		{
			ed::vector<Intersection> intersections;

			if (hit(segment, this->root.getBoundingBox(), removeVertexDistance))
			{
				intersections.reserve(100);
				this->root.findIntersections(segment, intersections, std::max(removeVertexDistance, 0.0));
			}

			return intersections;
		}

		ed::vector<Segment>& getSegments()
		{
			return this->segments;
		}

		int32_t height()
		{
			return this->root.height();
		}
	};
}

#endif