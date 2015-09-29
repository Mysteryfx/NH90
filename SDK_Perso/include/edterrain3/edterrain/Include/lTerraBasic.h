#ifndef __TERRA_BASIC_H__
#define __TERRA_BASIC_H__

#include "landscape3/lSemantics.h"
#include <stdint.h>

// USED NAMESPACES
#define NM_MATERIAL material3
#define NM_SURFACE surface3
#define NM_GEOMETRY geometry3
#define NM_SUPERFICIAL superficial3
#define NM_MODEL models3
#define NM_ROAD roads3

#define NM_MATERIAL_material3
#define NM_SURFACE_surface3
#define NM_GEOMETRY_geometry3
#define NM_SUPERFICIAL_superficial3
#define NM_MODEL_models3
#define NM_ROAD_roads3

namespace NM_SURFACE
{
	struct Square;
	struct lSurfaceFile;
}
namespace NM_GEOMETRY
{
	struct GeometryItem;
	struct GeometrySource;
}
namespace NM_SUPERFICIAL
{
	struct lSuperficial;
	struct lSuperficialFile;
}
namespace NM_MODEL
{
	class lModel;
	struct File;
}
namespace NM_ROAD
{
	struct lRoadFile;
}

#include <osg/Vec2i>
#include <osg/BoundingBox>
#include "edTerrain.h"
#include "cLinear.h"
#include "cPosition.h"
#include "GraphMath/Box.h"
#include <ed/string.h>

#pragma pack(1)

const float lSqrSize = 10000.f;
const float lSqrSize_2 = lSqrSize/2.f;
const float lLandZeroSquareX = -lSqrSize * 10.0f;
const float lLandZeroSquareZ = -lSqrSize * 16.0f;

// Имя 
namespace edterrain
{
	// Имя квадрата в номенклатуре: AA00
	EDTERRAIN_API ed::string getSquareName(int x, int z);

	// Имя квадрата в номенклатуре: m1_AA00
	EDTERRAIN_API ed::string getSquareName(int x, int z, int pixelsize);

	// индекс квадрата куда попадает точка. squaresize = 1, 2, 4, 8, 16, ...
	inline osg::Vec2i getSquareIndex( const osg::Vec3d& pt, int squaresize);
	// Баунд квадрата по индексу. squaresize = 1, 2, 4, 8, 16, ...
	inline osg::BoundingBoxf getSquareBound(int x, int z, int squaresize);

	// итератор по квадратам для по области заданой индексами SW, NE
	struct RectOfSquare
	{
		osg::Vec2i SW, NE;
		int count;
	public:
		RectOfSquare();
		inline void init(const osg::Vec2i& SW, const osg::Vec2i& NE);
		inline int size();
		inline osg::Vec2i operator [](int i);
	};
	// разбиение квадрата на 4
	inline RectOfSquare subSquares(osg::Vec2i sqind);
}

// Используется для указания квадрата
struct intpoint
{
	short x, z;
	intpoint(){x=0, z=0;}
	intpoint(int x, int z){ this->x=x; this->z=z;}
	intpoint(const intpoint& arg){x=arg.x, z=arg.z;}
	bool operator <(const intpoint& arg) const
	{
		if( x<arg.x) return true;
		if( x>arg.x) return false;
		return z<arg.z;
	}
	intpoint operator +(const intpoint& arg) const
	{
		intpoint res = *this;
		res.x += arg.x;
		res.z += arg.z;
		return res;
	}
	intpoint operator +=(const intpoint& arg)
	{
		x += arg.x;
		z += arg.z;
		return *this;
	}
	bool operator ==(const intpoint& arg) const
	{
		return (x==arg.x && z==arg.z);
	}
	bool operator !=(const intpoint& arg) const
	{
		return !(*this==arg);
	}
};

// x, z - точка
// level - степень 2 (0-10км, 1-20км, 2-40км, 3-80км, ...)
inline intpoint GetSquareIndex(float x, float z, int level=0)
{
	x = (x-lLandZeroSquareX)/lSqrSize;
	z = (z-lLandZeroSquareZ)/lSqrSize;
	intpoint res;
	res.x = (int)(floor(x))>>level;
	res.z = (int)(floor(z))>>level;
	return res;
}

inline osg::Vec2i GetSquareIndexOsg(float x, float z, int level = 0)
{
	x = (x - lLandZeroSquareX) / lSqrSize;
	z = (z - lLandZeroSquareZ) / lSqrSize;

	osg::Vec2i result;
	result.x() = (int)(floor(x)) >> level;
	result.y() = (int)(floor(z)) >> level;

	return result;
}

// Позиция квадрата
template <class VECTOR>
VECTOR& GetSquarePosition(intpoint squareindex, VECTOR& pos, int level=0)
{
	register int factor = 1<<level;
	pos.x = lLandZeroSquareX + (squareindex.x*lSqrSize + lSqrSize_2)*factor;
	pos.z = lLandZeroSquareZ + (squareindex.z*lSqrSize + lSqrSize_2)*factor;
	pos.y = 0;
	return pos;
}
// Позиция квадрата
template <class VECTOR>
VECTOR& GetSquarePositionOsg(intpoint squareindex, VECTOR& pos, int level=0)
{
	register int factor = 1<<level;
	pos.x() = lLandZeroSquareX + (squareindex.x*lSqrSize + lSqrSize_2)*factor;
	pos.z() = lLandZeroSquareZ + (squareindex.z*lSqrSize + lSqrSize_2)*factor;
	pos.y() = 0;
	return pos;
}

template <class osgVector>
osgVector& GetSquarePositionOsg(const osg::Vec2i& squareIndex, osgVector& pos, int level = 0)
{
	int factor = 1 << level;
	
	pos.x() = lLandZeroSquareX + (squareIndex.x() * lSqrSize + lSqrSize_2) * factor;
	pos.z() = lLandZeroSquareZ + (squareIndex.y() * lSqrSize + lSqrSize_2) * factor;
	pos.y() = 0;

	return pos;
}

// Длина стороны квадрата
inline float GetSquareEdgeSize(int level=0)
{
	register int factor = 1<<level;
	float size = lSqrSize*factor;
	return size;
}
// Бокс квадрата
inline Box& GetSquareBox(intpoint squareindex, Box& box, int level=0)
{
	Vector3 pos;
	GetSquarePosition(squareindex, pos, level);
	box = Box(pos, GetSquareEdgeSize(level));
	return box;
}

inline Box& GetSquareBox(const osg::Vec2i& squareIndex, Box& box, int level = 0)
{
	intpoint squareindex(squareIndex.x(), squareIndex.y());
	return GetSquareBox(squareindex, box, level);
}

// Индекс квадрата большего размера
inline intpoint GetOverSquareIndex(intpoint src, int leveldistance)
{
	src.x >>= leveldistance;
	src.z >>= leveldistance;
	return src;
}
inline intpoint GetOverSquareIndex_1(intpoint src, int leveldistance)
{
	src.x = ( (src.x-1)>>leveldistance ) + 1;
	src.z = ( (src.z-1)>>leveldistance ) + 1;
	return src;
}

typedef cPoint lv3V;
typedef cPoint ln3V;
struct  ltv2V
{
	float s,t; 
	ltv2V(){}
	ltv2V(float s, float t)
	{
		this->s = s;
		this->t = t;
	}
	bool operator ==(const ltv2V& arg) const
	{
		return memcmp( this, &arg, sizeof(arg))==0;
	}
	bool operator !=(const ltv2V& arg) const{return !(*this==arg);}
};
// Face
union li3V
{
	struct 
	{
		uint16_t i1,i2,i3;
	}i;
	uint16_t v[3];
	li3V operator +(int add)
	{
		li3V res = *this;
		res.i.i1+=add; 
		res.i.i2+=add;
		res.i.i3+=add;
		return res;
	}
	li3V operator -(int add)
	{
		li3V res = *this;
		res.i.i1-=add; 
		res.i.i2-=add;
		res.i.i3-=add;
		return res;
	}
	li3V()
	{
	}
	li3V(uint16_t i1, uint16_t i2, uint16_t i3)
	{
		this->i.i1 = i1;
		this->i.i2 = i2;
		this->i.i3 = i3;
	}
	bool operator ==(const li3V& arg) const
	{
		return memcmp( v, arg.v, sizeof(v))==0;
	}
	bool operator !=(const li3V& arg) const{return !(*this==arg);}
};
// Line segment
union li2V
{
	struct 
	{
		uint16_t i1,i2;
	}i;
	uint16_t v[2];

	li2V operator +(int add)
	{
		li2V res = *this;
		res.i.i1+=add; 
		res.i.i2+=add;
		return res;
	}
	li2V operator -(int add)
	{
		li2V res = *this;
		res.i.i1-=add; 
		res.i.i2-=add;
		return res;
	}
	/*/
	li2V& operator =(const li2V& arg)
	{
		this->i.i1 = arg.i.i1;
		this->i.i2 = arg.i.i2;
		return *this;
	}
	/*/
	li2V()
	{
	}
	li2V(uint16_t i1, uint16_t i2)
	{
		this->i.i1 = i1;
		this->i.i2 = i2;
	}
	bool operator ==(const li2V& arg) const
	{
		return memcmp( v, arg.v, sizeof(v))==0;
	}
	bool operator !=(const li2V& arg) const{return !(*this==arg);}
};


const double	lRegionSize = 20000.0f;
const double	lRegionSize1 = 1.f / lRegionSize;
const double	lRegionSize2 = lRegionSize * 0.5f;
const int		lRegSqrs = 2; 

const double	lSqrSize1 = 1.f / lSqrSize;
const double	lSqrSize2 = lSqrSize * 0.5;

// Размер квадрата по уровню детализации
int EDTERRAIN_API GetLevelSize(int level);

// Регион
void EDTERRAIN_API GetRegion(double _north, double _east, int &ix, int &jz, cVector &pos);
intpoint EDTERRAIN_API GetRegion(double _north, double _east, cVector &pos);

// Квадрат в регионе
void EDTERRAIN_API GetSquare(double x, double z, int &ix, int &jz, cVector &pos);
intpoint EDTERRAIN_API GetSquare(double x, double z, cVector &pos);

// Абсолютная ссылка на квадрат
intpoint EDTERRAIN_API GetAbsoluteSquare(double x, double z, cVector* pos=NULL);
intpoint EDTERRAIN_API GetAbsoluteSquare(double x, double z, osg::Vec3f* pos);
// Абсолютная ссылка на регион
intpoint EDTERRAIN_API GetAbsoluteRegion(double x, double z, cVector* respos=NULL);

// Проверка точка на границе?
// Вернет 0 если нет (иначе см. ниже)
int IsOnSquareSide(int iSize, const cPoint& pt1, const cPoint& pt2, float& param1, float& param2);

// Получить соседа по индексу
//			0 - запад
//			1 - восток
//			2 - юг
//			3 - север
intpoint EDTERRAIN_API Neighbour(intpoint p, short side);		

// Позиция центра квадрата
cVector EDTERRAIN_API GetSquarePosition(intpoint squareindex);
osg::Vec3f EDTERRAIN_API GetSquarePositionOsg(intpoint squareindex);

// Позиция центра квадрата
cVector EDTERRAIN_API GetRegionPosition(intpoint regionindex);


// Версия файлов хранилищ объектов
// Для проверки валидности ShapeQuadFileStorage
// и пр.
struct ObjectCollection_Version
{
	FILETIME writetime;
	
	ObjectCollection_Version()
	{
		writetime.dwLowDateTime = writetime.dwHighDateTime = 0;
	}

	bool operator ==(const ObjectCollection_Version& arg)
	{
		if( writetime.dwLowDateTime != arg.writetime.dwLowDateTime) return false;
		return (writetime.dwHighDateTime == arg.writetime.dwHighDateTime);
	}
};

// Типы позиционирования объектов
enum TYPE_POSITINING
{
	TP_BYNORMAL=1,		// пересчитать систему координат, учитывая наклон поверхности
	TP_ONLYHEIGTH=2,	// Оси нормируются, объект ставится на высоту земли
	TP_XZ_PLANE=3,		// проецируем на плоскость грани вектора X и Z cPosition
	TP_NO=4,			// Без трансформаций
	TP_REF_HEIGTH=5		// Без трансформаций, только к высоте добавляется высота поверхности
};

// Позиционировать объект на земле
class Box;
EDTERRAIN_API void lTerraPositioning(
	TYPE_POSITINING tp,		// Тип позиционирования объекта
	cPosition& pos,			// Позиция объекта
	const cVector& facet_normal, // Нормаль в точке объекта
	float surfaceHeigth,	// Высота в точке объекта
	const Box& box);		// Бокс модели

#pragma pack()

#include "lTerraBasic.inl"

#endif