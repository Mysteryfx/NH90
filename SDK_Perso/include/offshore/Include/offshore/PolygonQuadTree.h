#ifndef POLYGON_QUADTREE_H
#define POLYGON_QUADTREE_H

#include "../offshore_config.h"
#include "osg/TriangleUtils.h"

namespace offshore
{
	namespace PolygonQuadTreeUtils
	{
		inline bool isInside(const osg::Vec3d& vertex, const ed::vector<osg::Vec3d>& vertices, const offshore::Polygon& poly, int& outFace, osg::Vec3d& baryCoords, double trh)
		{
			return poly.isPointInside(vertex, vertices, outFace, baryCoords, trh);
		}

		inline bool isInside(const osg::Vec3d& vertex, const ed::vector<osg::Vec3d>& vertices, const offshore::Face& face, int& outFace, osg::Vec3d& baryCoords, double trh)
		{
			outFace = 0;
			return ed::isPointInsideTriangle(vertex, vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]], baryCoords, trh);
		}

		inline void expandBox(const ed::vector<osg::Vec3d>& vertices, const offshore::Polygon& poly, osg::BoundingBox& bbox)
		{
			for (size_t i=0; i<poly.indicies.size(); i++)
				bbox.expandBy(vertices[poly.indicies[i]]);
		}

		inline void expandBox(const ed::vector<osg::Vec3d>& vertices, const offshore::Face& face, osg::BoundingBox& bbox)
		{
			for (size_t i=0; i<3; i++)
				bbox.expandBy(vertices[face.v[i]]);
		}
	}

	template <typename Poly> class PolygonQuadTreeBase
	{
			const ed::vector<osg::Vec3d>& vertices;	
			const ed::vector<Poly>& polygons;
			ed::vector<osg::BoundingBox> bboxes;

			int32_t insertedPolyCount; 

			struct Node
			{
				const ed::vector<osg::Vec3d>& vertices;
				const ed::vector<osg::BoundingBox>& bboxes;
				const ed::vector<Poly>& polygons;
			
				double size;
				osg::Vec3d center;
			
				// Все полигоны ноды дерева разбиваем на две группы для последующего ускорения поиска
				ed::vector<int32_t> polygonIndiciesV;
				ed::vector<int32_t> polygonIndiciesH;
				osg::BoundingBox polygonBoxV, polygonBoxH;

				Node *childs[4];

				//================================================================================
				//
				//================================================================================
				Node(const ed::vector<osg::Vec3d>& vertices, const ed::vector<Poly>& polygons, const ed::vector<osg::BoundingBox>& bboxes, const osg::Vec3d& center, double size):
					vertices(vertices), polygons(polygons), bboxes(bboxes), center(center), size(size)
				{
					childs[0] = childs[1] = childs[2] = childs[3] = NULL;
					polygonIndiciesV.reserve(20);
					polygonIndiciesH.reserve(20);
					polygonBoxV.set(center, center);
					polygonBoxH.set(center, center);
				}

				//================================================================================
				// ~Node
				//    Очищаем память
				//================================================================================
				~Node()
				{
					if (childs[0] != NULL) delete childs[0];
					if (childs[1] != NULL) delete childs[1];
					if (childs[2] != NULL) delete childs[2];
					if (childs[3] != NULL) delete childs[3];
				}

				bool inside(const osg::BoundingBox& bbox, const osg::Vec3d& center, double size)
				{
					return bbox.xMax() <= center.x() + size && bbox.xMin() >= center.x() - size
						&& bbox.zMax() <= center.z() + size && bbox.zMin() >= center.z() - size;
				}

				void insert(const osg::BoundingBox& bbox, int32_t index)
				{
					double hsize = size / 2.0;

					osg::Vec3d center0 = osg::Vec3d(center.x() + hsize, 0.0, center.z() + hsize);
					osg::Vec3d center1 = osg::Vec3d(center.x() - hsize, 0.0, center.z() + hsize);
					osg::Vec3d center2 = osg::Vec3d(center.x() - hsize, 0.0, center.z() - hsize);
					osg::Vec3d center3 = osg::Vec3d(center.x() + hsize, 0.0, center.z() - hsize);

					if (inside(bbox, center0, hsize))
					{
						if (childs[0] == NULL) childs[0] = new Node(vertices, polygons, bboxes, center0, hsize);
						childs[0]->insert(bbox, index);
					}
					else if (inside(bbox, center1, hsize))
					{
						if (childs[1] == NULL) childs[1] = new Node(vertices, polygons, bboxes, center1, hsize);
						childs[1]->insert(bbox, index);
					}
					else if (inside(bbox, center2, hsize))
					{
						if (childs[2] == NULL) childs[2] = new Node(vertices, polygons, bboxes, center2, hsize);
						childs[2]->insert(bbox, index);
					}
					else if (inside(bbox, center3, hsize))
					{
						if (childs[3] == NULL) childs[3] = new Node(vertices, polygons, bboxes, center3, hsize);
						childs[3]->insert(bbox, index);
					}
					else
					{
						if ((bbox.xMin() - center.x()) * (bbox.xMax() - center.x()) < 0)
						{
							polygonIndiciesV.push_back(index);
							polygonBoxV.expandBy(bbox);
						}
						else
						{
							polygonIndiciesH.push_back(index);
							polygonBoxH.expandBy(bbox);
						}
					}
				}
				
				int32_t findPolygon(const osg::Vec3d& vertex, ed::vector<int32_t>& polygonIndicies, int32_t& outFace, osg::Vec3d& baryCoords)
				{
					for (size_t i = 0; i < polygonIndicies.size(); i++)
					{
						const osg::BoundingBox& bbox = bboxes[ polygonIndicies[i] ];

						if (bbox.contains(vertex))
						{
							const Poly& polygon = polygons[ polygonIndicies[i] ];

							if (PolygonQuadTreeUtils::isInside(vertex, vertices, polygon, outFace, baryCoords, 0.0))
								return polygonIndicies[i];
						}
					}

					return -1;
				}
				void queryPolygons(const osg::BoundingBox& box, ed::vector<int32_t>& polygons)
				{
					if (polygonBoxV.intersects(box))
					{
						for(int p=0; p<polygonIndiciesV.size(); p++)
						{
							int poly = polygonIndiciesV[p];
							if(this->bboxes[poly].intersects(box))
								polygons.push_back(poly);
						}
					}
					if (polygonBoxH.intersects(box))
					{
						for(int p=0; p<polygonIndiciesH.size(); p++)
						{
							int poly = polygonIndiciesH[p];
							if(this->bboxes[poly].intersects(box))
								polygons.push_back(poly);
						}
					}

					// check main box
					osg::Vec3d vsize(this->size, this->size, this->size);
					osg::BoundingBox fullbox(this->center-vsize, this->center+vsize);
					if (!fullbox.intersects(box))
						return;

					if (childs[0] != NULL) childs[0]->queryPolygons(box, polygons);
					if (childs[1] != NULL) childs[1]->queryPolygons(box, polygons);
					if (childs[2] != NULL) childs[2]->queryPolygons(box, polygons);
					if (childs[3] != NULL) childs[3]->queryPolygons(box, polygons);
				}

				int32_t insidePolygon(const osg::Vec3d& vertex, int32_t& outFace, osg::Vec3d& baryCoords)
				{
					int32_t polygonIndex = -1;

					if (polygonBoxV.contains(vertex))
						if ((polygonIndex = findPolygon(vertex, polygonIndiciesV, outFace, baryCoords)) >= 0)
							return polygonIndex;

					if (polygonBoxH.contains(vertex))
						if ((polygonIndex = findPolygon(vertex, polygonIndiciesH, outFace, baryCoords)) >= 0)
							return polygonIndex;
							
					osg::Vec3d delta = vertex - this->center;

					if (delta.x() >= 0.0) 
					{
						if (delta.z() >= 0.0) {
							if (childs[0] != NULL) polygonIndex = childs[0]->insidePolygon(vertex, outFace, baryCoords);
						} else {
							if (childs[3] != NULL) polygonIndex = childs[3]->insidePolygon(vertex, outFace, baryCoords);
						}
					} else {
						if (delta.z() >= 0.0) {
							if (childs[1] != NULL) polygonIndex = childs[1]->insidePolygon(vertex, outFace, baryCoords);
						} else {
							if (childs[2] != NULL) polygonIndex = childs[2]->insidePolygon(vertex, outFace, baryCoords);
						}
					}

					return polygonIndex;
				}
			};

			Node root;
		
		public:

			PolygonQuadTreeBase(const ed::vector<osg::Vec3d>& vertices, const ed::vector<Poly>& polygons, const osg::Vec3d& center, double size) :
				vertices(vertices), 
				polygons(polygons), 
				bboxes(polygons.size()), 
				insertedPolyCount(0),
				root(vertices, polygons, bboxes, center, size)
			{
				//...
			}

			PolygonQuadTreeBase(const ed::vector<osg::Vec3d>& vertices, const ed::vector<Poly>& polygons, const osg::BoundingBox& bbox) : 
				vertices(vertices),
				polygons(polygons), 
				bboxes(polygons.size()), 
				insertedPolyCount(0),
				root(vertices, polygons, bboxes, bbox.center(), bbox.radius())
			{
				//...
			}

			const ed::vector<osg::Vec3d>& getVertices() const 
			{
				return this->vertices;
			}

			const ed::vector<Poly>& getPolygons() const
			{
				return this->polygons;
			}

			void insert(int32_t polygonIndex)
			{
				const Poly& polygon = polygons[polygonIndex];

				if (bboxes.size() < polygons.size())
					bboxes.resize(polygons.size());

				osg::BoundingBox& bbox = bboxes[polygonIndex];

				// Вычисляем boundingBox для полигона
				PolygonQuadTreeUtils::expandBox(vertices, polygon, bbox);

				if(bbox.xMax()==bbox.xMin())
					return;
				if(bbox.zMax()==bbox.zMin())
					return;

				// Чтоб игнорировать y
				bbox.yMax() =  FLT_MAX;
				bbox.yMin() = -FLT_MAX;

				if (bbox.valid())
					root.insert(bbox, polygonIndex);

				++insertedPolyCount;
			}

			int32_t insidePolygon(const osg::Vec3d& vertex)
			{
				int32_t outFace;
				osg::Vec3d baryCoords;

				return root.insidePolygon(vertex, outFace, baryCoords);
			}

			int32_t insidePolygon(const osg::Vec3d& vertex, int32_t& outFace, osg::Vec3d& baryCoords)
			{
				return root.insidePolygon(vertex, outFace, baryCoords);
			}

			bool insidePolygon(const osg::Vec3d& point, offshore::PointOnMesh& ponm)
			{
				ponm.polygon = root.insidePolygon(point, ponm.face, ponm.barycentric);
				if (ponm.polygon < 0)
					return false;
				return true;
			}
			void queryPolygons(const osg::BoundingBox& box, ed::vector<int32_t>& polygons)
			{
				root.queryPolygons(box, polygons);
			}

			bool empty()
			{
				return insertedPolyCount == 0;
			}
	};

	typedef PolygonQuadTreeBase<offshore::Polygon> PolygonQuadTree;
	typedef PolygonQuadTreeBase<offshore::Face> FaceQuadTree;
}
#endif