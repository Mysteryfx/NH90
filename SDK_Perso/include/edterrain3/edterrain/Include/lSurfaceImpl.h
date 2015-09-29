#ifndef __lSurfaceImpl_h__
#define __lSurfaceImpl_h__

#include "lSurface.h"

#include <ed/map.h>

namespace ed
{
	class ITerrainPlugin;
}

namespace landscape
{
	////////////////////////////////////
	// Реализация lSurface
	class EDTERRAIN_API lSurfaceImpl : public lSurface
	{
	public:
		// ITerrainPlugin
		ed::ITerrainPlugin* pTerrainPlugin;

		// для управления объектами
		lSurfaceDataManager* pSurfaceDataManager;

		io::MmfStream mmfstream;
		surface3::lSurfaceFile file;

		ed::list<io::MmfStream> patches_mmfstream;
		ed::list<surface3::lSurfaceFile> patches_file;

		NM_SURFACE::lSurfaceFile* pSurfaceFile;

		// Список уже загруженых lShape
		typedef ed::map< NM_SURFACE::Square*, lShape*> tag_loadedshapes;
		tag_loadedshapes loadedshapes;

		// Список квадратов 10х10
		typedef ed::map<intpoint, lSurface::PLSquare> tag_squarelist;
		tag_squarelist squarelist;
		// Массив квадратов
		intpoint min, max;
		int sqarrayxSize;
		lSurface::PLSquare* sqarray;
		// Список регионов 20х20
		typedef ed::map<intpoint, lSurface::PLRegion> tag_regionlist;
		tag_regionlist regionlist;

		// Версия исходных данных
		FILETIME sourceversion;

		// Стандартные квадраты и регионы
		typedef ed::map<int, lSurface::PLShape> tag_lshapetemplatemap;
		tag_lshapetemplatemap squaretemplates;
		tag_lshapetemplatemap regiontemplates;

		/*/
		// Стандартные квадраты
		PLShape getSquareTemplate(intpoint squareindex, int level);
		PLShape getRegionTemplate(intpoint squareindex, int level);
		/*/

		// Загрузка квадрата по требованию
		lSurface::lSquare& LoadSquare(intpoint squareindex); 
		// Загрузка региона по требованию
		lSurface::lRegion& LoadRegion(intpoint squareindex); 

	public:
		lSurfaceImpl(ed::ITerrainPlugin* plugin=0);
		virtual ~lSurfaceImpl();

		// Инициализация
		virtual void Init(
			lSurfaceDataManager* pSurfaceDataManager, 
			const char* filename);
		// вгрузить патч поверхности (lsa3)
		virtual bool InitPatch(
			const char* filename
			);
		// 
		virtual void Exit();
		// 
		virtual void Release();

		// Версия исходных данных
		virtual void GetSourceVersion(FILETIME& filetime);

		// Диапазон квадратов
		virtual void getSquaresBound(intpoint& min, intpoint& max);
		// Диапазон регионов
		virtual void getRegionsBound(intpoint& min, intpoint& max);

		// Доступ к квадрату 10x10
		virtual lSurface::lSquare& GetSquare(intpoint squareindex);
		// Доступ к региону 20x20
		virtual lSurface::lRegion& GetRegion(intpoint squareindex);

		// Исходные данные
		virtual NM_SURFACE::Square* getSquare(const osg::Vec2i& squareIndex, int level);
		/* deprecated */virtual NM_SURFACE::Square* getSquare(intpoint squareindex, int level);

		// Высота квадрата
		virtual float getDirtyHeight(intpoint squareindex, int level);
		// Файл поверхности
		virtual NM_SURFACE::lSurfaceFile* getSurfaceFile(){return pSurfaceFile;};

		// Получить высоту по координате
		float getHeight(double x, double z);
		// Получить приблизительную высоту по координате
		float getDirtyHeight(float x, float z, float dirtySize=1000);
		float getDirtyHeight2(float x, float z, float dirtySize=1000);

		// Получить параметры треугольника
		void getFaceSpecs(double x, double z, dPoint &a, dPoint &b, dPoint &c, cVector &n);
		void getFaceSpecs(intpoint squareindex, unsigned int face, dPoint &a, dPoint &b, dPoint &c, cVector &n);
		void getFaceSpecs(intpoint squareindex, unsigned int face, osg::Vec3d &a, osg::Vec3d &b, osg::Vec3d &c, osg::Vec3d &n);
		void getFaceId(double x, double z, unsigned int& face, intpoint& squareindex);
		const char* getFaceSemantic(intpoint squareindex, unsigned int face);

		// Получить тип треугольника
		unsigned int getFaceType(double x, double z);

		// Найти точку пересечения отрезка с поверхностью
		bool segmentIntersect(const dPoint& a, const dPoint& b, dPoint *pos = 0);
		bool segmentIntersect(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d *pos = 0);

		// Построить разрез поверхности вдоль отрезка, возвращает количество точек в env
		int envelopeSurface(const dPoint& a, const dPoint& b, cPoint *env, int size);
		int envelopeSurface(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d *env, int size);

		// Найти ближайшую точку на море или на суше
		bool findClosest(const dVector& a, dVector& result, bool bWater, float maxDistance = 1000.f);

		// Найти ближайшую точку на суше, с достаточно ровной поверхностью (нужно для размещения вертолётных площадок)
		// alpha - максимальный допустимый угол наклона площадки в радианах
		bool findClosestFlat(const dVector& a, dVector& result, float alpha, float maxDistance = 1000.f);

		// Создать LShape
		lShape* CreateLShape(NM_SURFACE::Square* pSquare);
		// Файл земли
//		LandAreaFile& GetLandAreaFile(){ return theLandAreaFile;};
	private:
		
		// алгорити поиска следующей ближайшей точки
		bool nextPoint(dVector &vec, float &radius, float &step, float maxDistance);
	};
}


#endif