#ifndef LANDSCAPE4_LSURFACE_FILE_H
#define LANDSCAPE4_LSURFACE_FILE_H

//#define DEBUG_BSP

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lGeometry.h"
#include "landscape4/lDataFile.h"
#include "landscape4/BSPTree.h"
#include "landscape4/SpatialTree.h"
#include <osg/Vec2i>
#include <osg/Vec2d>
#include <osg/Vec3d>
#include <osg/Matrixd>
#include "lTerraDispatch.h"

namespace landscape4
{
	class lSurfaceFile : public lDataFile
	{
	public:

		struct Geometry : public geometry4::GeometrySource
		{
			material3::Material material;
		};

		struct Square
		{
			osg::Matrixf transform;

			// Баундинг бокс квадрата в Object Space
			osg::BoundingBoxf box;

			Geometry geometry;
			ed::vector<Geometry> superficials;

			landscape4::BSPTree2 bspTree;
		};

		ed::vector<Square> squares;

		landscape4::SpatialTree<uint32_t> spatialTree;

		virtual ed::string getType() { return "landscape4::lSurfaceFile"; }
		virtual bool copyFrom(lDataFile* src){*this = *(lSurfaceFile*)src;return true;};
		virtual bool isValid(){ return !squares.empty();};

	public:
		EDTERRAIN_API virtual ~lSurfaceFile();

		EDTERRAIN_API bool save(const char* filename);
		EDTERRAIN_API bool save(const ed::string& filename);
		EDTERRAIN_API bool load(io::MmfStream& stream);

	private:

		EDTERRAIN_API bool serialize(io::MmfStream& stream);
	};


#ifdef DEBUG_BSP
	struct LPDebugInfo
	{
		struct Edge
		{
			osg::Vec3d p1;
			osg::Vec3d p2;
		};
		ed::vector<osg::Vec3d> points;
		ed::vector<double> bar;
		ed::vector<Edge> edges;
		ed::string helpString;

		LPDebugInfo& operator+=(const LPDebugInfo& b)
		{
			points.insert(points.end(), b.points.begin(), b.points.end());
			bar.insert(bar.end(), b.bar.begin(), b.bar.end());
			edges.insert(edges.end(), b.edges.begin(), b.edges.end());
			helpString+=b.helpString;
			return *this;
		}
	};
#endif
	// landscape4::lPointOnSurface 
	// Враппер для запросов к земле:
	// getPoint(), getHeight(), getType(), intersection(), getNormal(), envelope()
	// 
	class lPointOnSurface 
	{
	public:
		lPointOnSurface();
		lPointOnSurface(lSurfaceFile* pSurfaceFile);  //Нужно для тестов
		double seaLevel;
		void init(double seaLevel);
		// Задать исследуемую точку
		void set(const osg::Vec3d& pt);
		void set(double x, double z);

	// Параметры исследуемой точки
	public:
		// Точка на поверхности
		osg::Vec3d getPoint() {return point;};
		// Запрос высоты
		double getHeight() {return getPoint().y();};
		// Тип поверхности
		uint32_t getType();
		// Нормаль поверхности в точке
		osg::Vec3d getNormal();
		//Номер квадрата точки
		uint32_t getSquareNumber();
		//Номер полигона точки в квадрате
		uint32_t getSquarePolygonNumber();
	// Операции
	public:
		// Пересечение с отрезком
		EDTERRAIN_API bool intersection(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d* res=0
#ifdef DEBUG_BSP
																									, LPDebugInfo* ip_debug = 0
#endif
																																);
		// Построить разрез поверхности вдоль отрезка
		void envelope(const osg::Vec3d& a, const osg::Vec3d& b, ed::vector<osg::Vec3d>& envelopePolyline);

	protected:
		class QueryPointSink : public landscape4::SpatialTree<uint32_t>::ISink
		{
		private:
			const osg::Vec3d& point;
			lPointOnSurface* processor;
		public:
			QueryPointSink(lPointOnSurface* processor, const osg::Vec3d& point) : point(point), processor(processor) {};
			virtual void onQuery(const uint32_t& object, const osg::BoundingBox& bbox)
			{
				processor->processSquarePoint(object, point);
			}
		};
		class QueryRaySink : public landscape4::SpatialTree<uint32_t>::ISink
		{
		private:
			const osg::Vec3d& pA;
			const osg::Vec3d& pB;
			lPointOnSurface* processor;
		public:
			bool result;
			osg::Vec3d point;
#ifdef DEBUG_BSP
			LPDebugInfo ip_debug;
#endif
			uint16_t v1;
			uint16_t v2;
			uint16_t v3;
			uint32_t polygon;
			uint32_t square;
			QueryRaySink(lPointOnSurface* processor, const osg::Vec3d& pA, const osg::Vec3d& pB) : pA(pA), pB(pB), processor(processor), result(false) {};
			virtual void onQuery(const uint32_t& object, const osg::BoundingBox& bbox)
			{
				uint16_t currentV1;
				uint16_t currentV2;
				uint16_t currentV3;
				uint32_t currentPolygon;
				osg::Vec3d currentPoint;
#ifdef DEBUG_BSP
				LPDebugInfo cip_debug;
				char tmp[100];
				sprintf(tmp, "Square %i ", object);
				ip_debug.helpString+=tmp;
#endif
				if (processor->processSquareRay(object, pA, pB, currentPoint, currentPolygon, currentV1, currentV2, currentV3
#ifdef DEBUG_BSP
																																, cip_debug
#endif																																
																																			))
				{
#ifdef DEBUG_BSP
					ip_debug.helpString+="+\n";
#endif
					if (!result||((currentPoint-pA).length()<(point-pA).length()))
					{
						square = object;
						point = currentPoint;
						v1 = currentV1;
						v2 = currentV2;
						v3 = currentV3;
						polygon = currentPolygon;
#ifdef DEBUG_BSP
						ip_debug += cip_debug;
#endif
					}
					result = true;
				}
#ifdef DEBUG_BSP
				else
					ip_debug.helpString+="-\n";
#endif
			}
		};

		// кеш предыдущего запроса
		lSurfaceFile* pSurfaceFile;
		osg::Vec3d sourcePoint;
		lSurfaceFile::Square* square;
		uint16_t v1;
		uint16_t v2;
		uint16_t v3;
		uint32_t squareNumber;
		uint32_t polygonNumber;
		osg::Vec3d point;
		osg::Vec3d normal;
		uint32_t pointType;

		// update cache
		EDTERRAIN_API void updateCache(const osg::Vec3d& pt);

		friend class QueryPointSink;
		EDTERRAIN_API void processSquarePoint(uint32_t sq, const osg::Vec3d& pt);
		friend class QueryRaySink;
		EDTERRAIN_API bool processSquareRay(uint32_t sq, const osg::Vec3d& pA, const osg::Vec3d& pB, osg::Vec3d& result, uint32_t& polygon, uint16_t& v1, uint16_t& v2, uint16_t& v3
#ifdef DEBUG_BSP
																																													, LPDebugInfo& ip_debug
#endif
																																																			);
	};
}

#include "inl/SurfaceFile.inl"

#endif