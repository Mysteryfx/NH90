#ifndef BOUNDING_BOX_WRAP_OCTREE_H
#define BOUNDING_BOX_WRAP_OCTREE_H

#include "BoundingBoxOctree.h"
#include "BoxUtils.h"

namespace ed
{
	template <typename T> class BoundingBoxWrapOctree
	{
	public:
		
		/**
		 * Параметром конструктора передается величина wrap'а
		 * Если величина по какой-то оси <=0, то по этой оси wrap не производится
		*/
		BoundingBoxWrapOctree() {};
		BoundingBoxWrapOctree(osg::Vec3d wrap) : wrap(wrap) {};

		struct IQuerySink
		{
			virtual void onQuery(const T& t, const osg::Vec3d& pos, const osg::BoundingBox& box, const osg::Vec3d& boxOfs)  = 0;
		};

		void clear() {internalOctree.clear();};

		/**
		 * Вставить объект в дерево с четко заданным баундом
		 */
		void insert(const T& object, const osg::BoundingBox& bbox)
		{
			osg::Vec3d center = bbox.center();
			osg::Vec3d wrapOfs = getWrapOffset(center);
			osg::BoundingBox nBBox(bbox);
			ed::moveBboxTo(nBBox, wrapOfs);
			internalOctree.insert(std::make_pair(wrapOfs, object), nBBox);
		}


		/**
		 * Выбрать объекты, попадающих во фрастум
		 */
		void query(const osg::Polytope& frustum, ed::vector<std::pair<T, osg::Vec3d> >& queriedObjects, ed::vector<std::pair<const osg::BoundingBox*, osg::Vec3d> >* queriedBoxes = NULL)
		{
			if (queriedBoxes)
			{
				BBoxCollectToArraySink sink(queriedObjects, *queriedBoxes);
				query(frustum, &sink);
			}
			else
			{
				CollectToArraySink sink(queriedObjects);
				query(frustum, &sink);
			}
		}

		void query(const osg::BoundingBox& frustum, ed::vector<std::pair<T, osg::Vec3d> >& queriedObjects, ed::vector<std::pair<const osg::BoundingBox*, osg::Vec3d> >* queriedBoxes = NULL)
		{
			if (queriedBoxes)
			{
				BBoxCollectToArraySink sink(queriedObjects, *queriedBoxes);
				query(frustum, &sink);
			}
			else
			{
				CollectToArraySink sink(queriedObjects);
				query(frustum, &sink);
			}
		}
	
		/**
		 * Выбрать объекты, попадающие во фрастум или в баундинг бокс
		 */
		void query(const osg::Polytope& frustum, IQuerySink* sink)
		{
			queryInternal(ed::boxOfFrustum(frustum), frustum, sink);
		}

		void query(const osg::BoundingBox& box, IQuerySink* sink)
		{
			queryInternal(box, box, sink);
		}
	private:
		// Сбор объектов в список
		struct CollectToArraySink : IQuerySink
		{
			ed::vector<std::pair<T, osg::Vec3d> >& queriedObjects;
			CollectToArraySink(ed::vector<std::pair<T, osg::Vec3d> >& queriedObjects) : queriedObjects(queriedObjects) {};
			virtual void onQuery(const T& t, const osg::Vec3d& pos, const osg::BoundingBox& box, const osg::Vec3d& boxOfs)  { queriedObjects.push_back(std::make_pair(t, pos));}
		};

		// Сбор объектов в список c BBox
		struct BBoxCollectToArraySink : IQuerySink
		{
			ed::vector<std::pair<T, osg::Vec3d> >& queriedObjects;
			ed::vector<std::pair<const osg::BoundingBox*, osg::Vec3d> >& queriedBoxes;
			BBoxCollectToArraySink(ed::vector<std::pair<T, osg::Vec3d> >& queriedObjects, ed::vector<std::pair<const osg::BoundingBox*, osg::Vec3d> >& queriedBoxes) : queriedObjects(queriedObjects), queriedBoxes(queriedBoxes) {};
			virtual void onQuery(const T& t, const osg::Vec3d& pos, const osg::BoundingBox& box, const osg::Vec3d& boxOfs) 
			{ 
				queriedObjects.push_back(std::make_pair(t, pos));
				queriedBoxes.push_back(std::make_pair(&box, boxOfs));
			}
		};

		struct InternalSink : ed::BoundingBoxOctree<std::pair<osg::Vec3d, T> >::ISink
		{
			osg::Vec3d ofs;
			IQuerySink* sink;
			InternalSink(IQuerySink* sink) : sink(sink) {}
			virtual void onQuery(const std::pair<osg::Vec3d, T>& t, const osg::BoundingBox& box) {sink->onQuery(t.second, t.first-ofs, box, -ofs);}
		};

		ed::BoundingBoxOctree<std::pair<osg::Vec3d, T> > internalOctree;
		osg::Vec3d wrap;
		double calcWrapOffset(const double& wrap, const double& val) const
		{
			if (wrap<=0.0)
				return 0.0;
			return -floor(val/wrap)*wrap;
		}
		osg::Vec3d getWrapOffset(const osg::Vec3d& point) const
		{
			osg::Vec3d res;
			res.x() = calcWrapOffset(wrap.x(), point.x());
			res.y() = calcWrapOffset(wrap.y(), point.y());
			res.z() = calcWrapOffset(wrap.z(), point.z());
			return res;
		}
		osg::BoundingBox transformBorder(const osg::BoundingBox& border, osg::Vec3d ofs)
		{
			osg::BoundingBox res(border);
			ed::moveBboxTo(res, ofs);
			return res;
		}
		osg::Polytope transformBorder(const osg::Polytope& border, osg::Vec3d ofs)
		{
			osg::Polytope res;
			res.setAndTransformProvidingInverse(border, osg::Matrixd::translate(-ofs));
			return res;
		}
		template <typename O> void queryInternal(const osg::BoundingBox& bbox, const O& border, IQuerySink* sink)
		{
			InternalSink iSink(sink);
			ptrdiff_t dxMax = (wrap.x()>0.0)?(-floor(bbox.xMin()/wrap.x()-0.5)):0;
			ptrdiff_t dxMin = (wrap.x()>0.0)?(-ceil(bbox.xMax()/wrap.x()+0.5)):0;
			ptrdiff_t dyMax = (wrap.y()>0.0)?(-floor(bbox.yMin()/wrap.y()-0.5)):0;
			ptrdiff_t dyMin = (wrap.y()>0.0)?(-ceil(bbox.yMax()/wrap.y()+0.5)):0;
			ptrdiff_t dzMax = (wrap.z()>0.0)?(-floor(bbox.zMin()/wrap.z()-0.5)):0;
			ptrdiff_t dzMin = (wrap.z()>0.0)?(-ceil(bbox.zMax()/wrap.z()+0.5)):0;
			for (ptrdiff_t dx = dxMin; dx<=dxMax; dx++)
				for (ptrdiff_t dy = dyMin; dy<=dyMax; dy++)
					for (ptrdiff_t dz = dzMin; dz<=dzMax; dz++)
					{
						iSink.ofs = osg::Vec3d(dx*wrap.x(), dy*wrap.y(), dz*wrap.z());
						internalOctree.query(transformBorder(border, iSink.ofs), &iSink);
					}
		}
	};

}

#endif