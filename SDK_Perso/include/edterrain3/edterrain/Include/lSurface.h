#ifndef __lSurface_h__
#define __lSurface_h__

#include "misc/misc_error.h"
#include "Mem/Shared.h"
//#include "lLandscapeFileFormat.h"
//#include "lSuperficial.h"
#include "lTerraBasic.h"
#include "landscape3/lsurfacefile.h"

#define MAX_SEGMENT_LENGTH	1e6

namespace landscape
{
	class lSemanticSquare;
	class lSurfaceDataManager;
	////////////////////////////////////
	// Поверхность земли
	// класс для отрисовки и мат.операций
	class EDTERRAIN_API lSurface
	{
	public:
		////////////////////////////////////
		// Геометрия квадрата
		class EDTERRAIN_API lShape : public Mem::Shared
		{
		public:
			NM_SURFACE::Square* pSquare;
			lShape(){ pSquare=NULL;}
			virtual ~lShape(){};
			// Инициализация
			virtual void Init(){};
			virtual unsigned int GetFaceSemantic(int face){
				return GetSemanticType( pSquare->getFaceSemantic(face));};
		};
		typedef Mem::Ptr<lShape> PLShape;

		////////////////////////////////////
		// Квадрат поверхности 10x10
		class EDTERRAIN_API lSquare : public Mem::Shared
		{
		public:
			// Индекс квадрата
			intpoint squareindex;
			// Уровни детализации
			PLShape level0;			// Для мат. операций
			PLShape level1;
			// Проверка сематники накладных объектов
//			lSemanticSquare* superficialsemantic;

			lSquare(){};//superficialsemantic = (lSemanticSquare*)0xFFFFFFFF;};
			virtual ~lSquare();
			lShape& getShape0()
			{
				lShape* pshape = level0.get();
				return *pshape;
			}
			// Инициализация
			virtual void Init(){};
			// Инициализация superficialsemantic
			virtual void InitSS();
			// Семантика
			virtual bool IsPointOn(float x, float z, int semantic);
		};	
		typedef Mem::Ptr<lSquare> PLSquare;

		////////////////////////////////////
		// Квадрат поверхности 20x20
		class EDTERRAIN_API lRegion : public Mem::Shared
		{
		public:
			lRegion(){};
			virtual ~lRegion(){};
			// Индекс региона
			intpoint regionindex;
			// Уровни детализации
			PLShape level2;
			PLShape level3;
			// Квадраты
			PLSquare squares[2][2];
			// Инициализация
			virtual void Init(){};
		};
		typedef Mem::Ptr<lRegion> PLRegion;

	public:
		// Инициализация
		virtual void Init(
			landscape::lSurfaceDataManager* pSurfaceDataManager, 
			const char* filename)=0;

		// вгрузить патч поверхности (lsa3)
		virtual bool InitPatch(
			const char* filename
			)=0;

		// 
		virtual void Exit()=0;
		// 
		virtual void Release()=0;

		// Версия исходных данных
		virtual void GetSourceVersion(FILETIME& filetime)=0;

		// Диапазон квадратов
		virtual void getSquaresBound(intpoint& min, intpoint& max)=0;
		// Диапазон регионов
		virtual void getRegionsBound(intpoint& min, intpoint& max)=0;

		// Доступ к квадрату 10x10
		virtual lSurface::lSquare& GetSquare(intpoint squareindex)=0;
		// Доступ к региону 20x20
		virtual lSurface::lRegion& GetRegion(intpoint squareindex)=0;

		// Исходные данные
		virtual NM_SURFACE::Square* getSquare(const osg::Vec2i& squareIndex, int level) = 0;
		/* deprecated */virtual NM_SURFACE::Square* getSquare(intpoint squareindex, int level) = 0;

		// Высота квадрата
		virtual float getDirtyHeight(intpoint squareindex, int level)=0;
		// Файл поверхности
		virtual NM_SURFACE::lSurfaceFile* getSurfaceFile()=0;
		// Геометрия
		
		// Получить высоту по координате
		virtual float getHeight(double x, double z) = 0;
		// Получить приблизительную высоту по координате
		virtual float getDirtyHeight(float x, float z, float dirtySize=1000) = 0;
		virtual float getDirtyHeight2(float x, float z, float dirtySize=1000) = 0;

		// Получить параметры треугольника
		virtual void getFaceSpecs(double x, double z, dPoint &a, dPoint &b, dPoint &c, cVector &n) = 0;
		virtual void getFaceSpecs(intpoint squareindex, unsigned int face, dPoint &a, dPoint &b, dPoint &c, cVector &n) = 0;
		virtual void getFaceSpecs(intpoint squareindex, unsigned int face, osg::Vec3d &a, osg::Vec3d &b, osg::Vec3d &c, osg::Vec3d &n) = 0;
		virtual void getFaceId(double x, double z, unsigned int& face, intpoint& squareindex)=0;
		virtual const char* getFaceSemantic(intpoint squareindex, unsigned int face)=0;

		// Получить тип треугольника
		virtual unsigned int getFaceType(double x, double z) = 0;

		// Найти точку пересечения отрезка с поверхностью
		virtual bool segmentIntersect(const dPoint& a, const dPoint& b, dPoint *pos = 0) = 0;
		virtual bool segmentIntersect(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d *pos = 0) = 0;

		// Построить разрез поверхности вдоль отрезка, возвращает количество точек в env
		virtual int envelopeSurface(const dPoint& a, const dPoint& b, cPoint *env, int size) = 0;
		virtual int envelopeSurface(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d *env, int size) = 0;

		// Найти ближайшую точку на море или на суше
		virtual bool findClosest(const dVector& a, dVector& result, bool bWater, float maxDistance = 1000.f)=0;

		// Найти ближайшую точку на суше, с достаточно ровной поверхностью (нужно для размещения вертолётных площадок)
		// alpha - максимальный допустимый угол наклона площадки в радианах
		virtual bool findClosestFlat(const dVector& a, dVector& result, float alpha, float maxDistance = 1000.f)=0;

	public:
		// Хелперы для координат с одинарной точностью
		virtual void getFaceSpecs(double x, double z, cPoint &a, cPoint &b, cPoint &c, cVector &n);
		virtual void getFaceSpecs(intpoint squareindex, unsigned int face, cPoint &a, cPoint &b, cPoint &c, cVector &n);
		virtual bool segmentIntersect(const dPoint& a, const dPoint& b, cPoint *pos);
		virtual bool findClosest(const dVector& a, cVector& result, bool bWater, float maxDistance = 1000.f);
		virtual bool findClosestFlat(const dVector& a, cVector& result, float alpha, float maxDistance = 1000.f);
	};

	////////////////////////////////////
	// Интерфейс для управления об\тами поверхности
	class EDTERRAIN_API lSurfaceDataManager
	{
	public:
		virtual lSurface::lShape*  CreateLShape()=0;
		virtual lSurface::lSquare* CreateLSquare()=0;
		virtual lSurface::lRegion* CreateLRegion()=0;
	};

//	EDTERRAIN_API lSurface* CreateLandSurface();
//	EDTERRAIN_API void SetLandSurface(lSurface*);
//	EDTERRAIN_API lSurface* GetLandSurface();


}

#endif