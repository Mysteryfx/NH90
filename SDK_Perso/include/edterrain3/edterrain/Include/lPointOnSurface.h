#ifndef _LPOINTONSURFACE_H_
#define _LPOINTONSURFACE_H_
#include "edTerrain.h"
#include "landscape3/lPathFile.h"

//#include "paths/lPathFile.h"
//#include "math_bmpMatrixMN.h"

/////////////////////////////////////////////////////////////////
// Точка на поверхности земли
// Используется для расчета:
// - квадрата
// - высоты
// - нормали поверхности
// - семантики тр-ка
// - ...
namespace ed
{
	class ITerrainPlugin;
}

namespace landscape
{
	class EDTERRAIN_API lPointOnSurface
	{
	private:
//		lSurface* Land;

		// Координата y расчитываемая!
		dPoint xyz; // в мировых координатах
		cPoint lxyz; // в локальных координатах

		// square definition
		intpoint sqPos;

		ed::ITerrainPlugin* pTerrainPlugin;
		NM_SURFACE::Square* pSquare;
		
		// Точка на дороге
		NM_ROAD::PointOnRoad por;

		// Расчитываемые значения
		unsigned int face;	// triangle of this square 
		cVector normal;			// Нормаль в точке
		unsigned int surfType;	// Семантика тр-ка
		// Семантики накладных
		bool bOnRunway;			// на ВПП
		bool bOnWater;			// на Воде
		bool bShallow;			// на мелкой воде
		bool bOnAutoRoad;		// на дороге
		bool bInCity;
		bool bInForest;

		// Признаки валидности
		bool bFaceValid;
		bool bMathValid;
		int checksemanticValid;		// 1-ВПП, 2-Вода, 4-AutoRoad, 8-Shallow
		
		void Invalid(void);     // unvalidate flags
		void CheckSquare(void); // get pSquare based on xyz
		void FindFace(void);    // fill face number and semantics
		void GetMath(void);     // fill normal and height

		void Clear();
	// osg support
	public:
		lPointOnSurface(const osg::Vec3d&);
		lPointOnSurface& operator=(const osg::Vec3d&);
		lPointOnSurface& operator+=(const osg::Vec3d&);
		lPointOnSurface& operator-=(const osg::Vec3d&);

		osg::Vec3d operator +(const osg::Vec3d&);
		osg::Vec3d operator -(const osg::Vec3d&);
		// Точка
		operator const osg::Vec3d();

		// Позиция
		void getPoint(osg::Vec3d& v)const{v.x() = xyz.x; v.y() = 0; v.z() = xyz.z; }

		// Нормаль
		void getNormal(osg::Vec3d& normal);

		// пересечь отрезок в районе этой точки с поверхностью
		// точка переместится в точку пересечения
		bool intersectSurface(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d *p = 0);

		// Аналоги
		static bool VectorGroundIntersection(const osg::Vec3d& Pnt, const osg::Vec3d& Vec, float MaxDist, osg::Vec3d& point);
		static bool groundIntersection(const osg::Vec3d& from, const osg::Vec3d& to, osg::Vec3d& point);
		static bool isVisible(const osg::Vec3d& from, const osg::Vec3d& to);

	public:
		lPointOnSurface(void);

		// Исследуемая точка
		void set(double x, double z);
		void set(const osg::Vec3d& pt){set(pt.x(), pt.z());};

		lPointOnSurface& operator=(const dPoint&);
		lPointOnSurface& operator+=(const dVector&);
		lPointOnSurface& operator-=(const dVector&);

		dPoint operator +(const dVector&);
		dPoint operator -(const dVector&);

		// Точка
		operator const dPoint&();
		
	//////////////////////////////////////////////
	// Основные ф-ции доступа
	public:
		// Позиция
		void getPoint(dPoint& v)const{v.x = xyz.x; v.y = 0; v.z = xyz.z; }

		// Высота
		float getHeight();
		
		// Точка на дороге
		NM_ROAD::PointOnRoad pointOnRoad();

		// Нормаль
		void getNormal(cVector& normal);

		// пересечь отрезок в районе этой точки с поверхностью
		// точка переместится в a
		bool intersectSurface(const dPoint& a, const dPoint& b, dPoint *p = 0);

		//"крутизна" склона в радианах
		float slopeAngle();

	// Проверка без учета накладных объектов
	public:
		// Семантика (без учета накладных объектов)
		unsigned int getTypeSurface();
		// Вернет true если тип поверхности вода (без учета накладных объектов)
		bool isTypeSurfaceWater();
		bool isTypeSurfaceSea();

	//////////////////////////////////////////////
	// Проверка с учетом накладных объектов
	public:
		// На ВПП
		bool IsOnRunway();

		// Находимся ли мы над водой, с учетом накладных объектов
		bool IsOnWater();

		bool getRiverDepth(float &depth);
		// Находимся ли мы над мелкой водой (река, канал)
		bool IsShallowWater();
		//Извращённый метод чтобы машинки могли погружжаться в мелкие реки
		//Информация о реке представлена треугольником
		bool IsOnShallowRiver(NM_GEOMETRY::GeometryItem*& geom, unsigned long& faceIndex, dVector& sqPos);

		bool IsInCity();
		bool IsInForest();
	//////////////////////////////////////////////
	// Проверка по дорожной сети
	public:
		// Точка на дороге?
		bool IsOnRoad(double height = 0.);

	// Тестовые ф-ции
	public:
		// В BMP
//		static void BuildSemanticBmp(Box& box, math::bmpMatrix& bmp);
//		static void BuildSemanticBmp(Box& box, math::palBmpMatrix& bmp);

		void getSpec(intpoint& sqPos, unsigned int& face);
	// 
	public:
	};
}
#endif
