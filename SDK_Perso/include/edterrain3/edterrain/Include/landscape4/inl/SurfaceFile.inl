#include <limits>

namespace landscape4
{
	inline lPointOnSurface::lPointOnSurface() : sourcePoint(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), square(NULL), v1(0), v2(0), v3(0), squareNumber((uint32_t)-1), polygonNumber((uint32_t)-1), pointType(typeMeshLand), pSurfaceFile(NULL), seaLevel(0)
	{

	}
	inline lPointOnSurface::lPointOnSurface(lSurfaceFile* pSurfaceFile) : pSurfaceFile(pSurfaceFile), sourcePoint(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), square(NULL), v1(0), v2(0), v3(0), pointType(typeMeshLand), normal(0, 1, 0), seaLevel(0)
	{

	}
	inline void lPointOnSurface::init(double seaLevel)
	{
		this->seaLevel = seaLevel;
		pSurfaceFile = 0;
		lTerraDispatch* pTerraDispatch = GetTerraDispatch();
		if( pTerraDispatch)
		{
			pSurfaceFile = pTerraDispatch->GetSurfaceFile4();
		}
	}
	inline void lPointOnSurface::set(const osg::Vec3d& pt)
	{
		if ((sourcePoint.x()!=pt.x())||(sourcePoint.y()!=pt.y())||(sourcePoint.z()!=pt.z()))
		{
			sourcePoint = pt;
			updateCache(pt);
			if (point.y()<seaLevel)
			{
				point.y() = seaLevel;
				normal = osg::Vec3d(0, 1, 0);
				pointType = typeMeshSea;
			}
		}
	}
	
	inline void lPointOnSurface::set(double x, double z)
	{
		if ((sourcePoint.x()!=x)||(sourcePoint.z()!=z)||(sourcePoint.y()!=std::numeric_limits<double>::max()))
		{
			sourcePoint = osg::Vec3d(x, std::numeric_limits<double>::max(), z);
			updateCache(sourcePoint);
			if (point.y()<seaLevel)
			{
				point.y() = seaLevel;
				normal = osg::Vec3d(0, 1, 0);
				pointType = typeMeshSea;
			}
		}
	}

	// Тип поверхности
	inline uint32_t lPointOnSurface::getType()
	{
		return pointType;
	}
	// Нормаль поверхности в точке
	inline osg::Vec3d lPointOnSurface::getNormal()
	{
		return normal;
	}

	// Построить разрез поверхности вдоль отрезка
	inline void lPointOnSurface::envelope(const osg::Vec3d& a, const osg::Vec3d& b, ed::vector<osg::Vec3d>& envelopePolyline)
	{
		return;
	}

	inline uint32_t lPointOnSurface::getSquareNumber()
	{
		return squareNumber;
	}
	
	inline uint32_t lPointOnSurface::getSquarePolygonNumber()
	{
		return polygonNumber;
	}
}

