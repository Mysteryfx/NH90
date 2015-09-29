#ifndef __LANDSCAPE3_lPointOnSurface_H
#define __LANDSCAPE3_lPointOnSurface_H
#include "../landscape3_config.h"

#include <osg/Vec2i>
#include "edTerrain.h"
#include "lTerraBasic.h"
#include "landscape3/lSurfaceFile.h"

#ifdef EDGE
#else
#include "landscape3/lPathFile.h"
#endif

namespace ed
{
	class ITerrainPlugin;
}

namespace surface3
{
#ifdef EDGE




class EDTERRAIN_API lPointOnSurface
{
private:
	// Координата y расчитываемая!
	osg::Vec3d xyz; // в мировых координатах
	osg::Vec3f lxyz; // в локальных координатах

	// square definition
	intpoint sqPos;

	surface3::Square* pSquare;

	// Расчитываемые значения
	unsigned int face;	// triangle of this square
	osg::Vec3f normal;			// Нормаль в точке
	unsigned int surfType;	// Семантика тр-ка
	// Семантики накладных
	bool bOnRunway;			// на ВПП
	bool bOnWater;			// на Воде
	bool bShallow;			// на мелкой воде
	bool bOnAutoRoad;		// на дороге

	// Признаки валидности
	bool bFaceValid;
	bool bMathValid;
	int checksemanticValid;		// 1-ВПП, 2-Вода, 4-AutoRoad, 8-Shallow

	void Invalid(void);     // unvalidate flags
	void CheckSquare(void); // get pSquare based on xyz
	void FindFace(void);    // fill face number and semantics
	void GetMath(void);     // fill normal and height

	// osg support
public:
	// Аналоги
	static bool VectorGroundIntersection(const osg::Vec3d& Pnt, const osg::Vec3d& Vec, float MaxDist, osg::Vec3d& point);
	static bool groundIntersection(const osg::Vec3d& from, const osg::Vec3d& to, osg::Vec3d& point);
	static bool isVisible(const osg::Vec3d& from, const osg::Vec3d& to);

public:
	lPointOnSurface(void);

	// Исследуемая точка
	void set(double x, double z);
	void set(const osg::Vec3d& pos);
	void set(const osg::Vec3f& pos);

	lPointOnSurface& operator=(const osg::Vec3d&);
	lPointOnSurface& operator+=(const osg::Vec3d&);
	lPointOnSurface& operator-=(const osg::Vec3d&);

	osg::Vec3d operator +(const osg::Vec3d&);
	osg::Vec3d operator -(const osg::Vec3d&);

	// Точка
	operator const osg::Vec3d&();

	//////////////////////////////////////////////
	// Основные ф-ции доступа
public:
	// Позиция
	void getPoint(osg::Vec3d& v)const
	{
		v.x() = xyz.x();
		v.y() = 0;
		v.z() = xyz.z();
	}

	// Высота
	float getHeight();

	// Нормаль
	void getNormal(osg::Vec3f& normal);

	// пересечь отрезок в районе этой точки с поверхностью
	// точка переместится в a
	bool intersectSurface(const osg::Vec3d& a, const osg::Vec3d& b, osg::Vec3d* p = 0);

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

	bool getRiverDepth(float& depth);
	// Находимся ли мы над мелкой водой (река, канал)
	bool IsShallowWater();
	//Извращённый метод чтобы машинки могли погружжаться в мелкие реки
	//Информация о реке представлена треугольником
	bool IsOnShallowRiver(geometry3::GeometryItem*& geom, unsigned long& faceIndex, osg::Vec3d& sqPos);
	//////////////////////////////////////////////
	// Проверка по дорожной сети
public:

	// Тестовые ф-ции
public:

	void getSpec(intpoint& sqPos, unsigned int& face);
	//
public:
};

inline void lPointOnSurface::set(const osg::Vec3d& pos)
{
	this->set(pos.x(), pos.z());
}
inline void lPointOnSurface::set(const osg::Vec3f& pos)
{
	this->set(pos.x(), pos.z());
}
#else









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

	// Признаки валидности
	bool bFaceValid;
	bool bMathValid;
	int checksemanticValid;		// 1-ВПП, 2-Вода, 4-AutoRoad, 8-Shallow
		
	void Invalid(void);     // unvalidate flags
	void CheckSquare(void); // get pSquare based on xyz
	void FindFace(void);    // fill face number and semantics
	void GetMath(void);     // fill normal and height

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

#endif
}

#endif