#ifndef __LANDSCAPE3_LSUPERFICIAL_H
#define __LANDSCAPE3_LSUPERFICIAL_H
#include "../landscape3_config.h"

#include <osg/Vec2i>
#include "landscape3/lMaterial.h"
#include "landscape3/lGeometry.h"
//#include "math_bmpMatrixMN.h"

namespace superficial3
{
// Цель отрисовки
const unsigned int sdp_Land = 0x1;					// Отрисовка земли
const unsigned int sdp_Map  = 0x2;					// Отрисовка карты
const unsigned int sdp_Tex  = 0x4;					// Отрисовка в текстуру
const unsigned int sdp_TestSemantic = 0x80000000;	// Проверка семантики
const unsigned int sdp_Necessary    = 0x40000000;	// Признак необходимости для переключения LOD
const unsigned int sdp_Unused		 = 0x3FFF0000;	// Для служебных целей

const int current_version = 0;
const char current_filetype[4] = {'s', 'u', 'p', '2'};

// Квадрат
struct lSuperficial
{
	struct Item : public geometry3::GeometryItem
	{
		// индекс geometry source в файле
		uint32_t geometrysourceindex;

		// Цель см. флаги sdp_...
		uint32_t sdp_flags;
		// Иниц.
		Item& operator =(const geometry3::GeometryItem& src);
		// проверка
		bool operator ==(const Item& arg) const;
	};
	// Для отрисовки
	ed::vector< Item> geoms;
	// Для проверки ???
	//		ed::vector< geometry3::GeometryItem> test;

	// Функции наполнения
public:
	EDTERRAIN_API lSuperficial();
	EDTERRAIN_API lSuperficial(const lSuperficial& src);
	// Проверка совпадения
	bool match(lSuperficial& arg);
	// Построить информацию для проверки семантики
	void BuildTestInfo();
	// Загрузка
	void preload();

	// Инициализация после загрузки
	void InitAfterLoad(
	    ed::vector< material3::Material>& materials,	// Материалы
	    geometry3::GeometrySource* geometrysources
	);

};
// Квадрат для проверки семантики одного типа
struct lSemanticSquare
{
	ed::vector< geometry3::GeometryItem* > geoms;

	// для сохранения в файле
	uint32_t pSquares;		// номер квалрата
	ed::vector< uint32_t> geomsindecies;	// индексы геометрий в квадрате
	// Проверка
	EDTERRAIN_API bool Test(float x, float z, float* depth);
	EDTERRAIN_API bool Test(float x, float z);
	// Версия Test возвращающая треугольник где находится точка
	EDTERRAIN_API bool Test(float x, float z, geometry3::GeometryItem*& geom,
	                        unsigned long& faceIndex);

	// Инициализация после загрузки
	void InitAfterLoad(
	    lSuperficial* squarepool
	);
};

// Заголовок
struct lSuperficialFile
{
	// Материалы
	ed::vector< material3::Material> materials;
	// Geometry Sources
	ed::vector< geometry3::GeometrySource> geometrysources;
	// пул всех lSuperficial
	ed::vector<lSuperficial> squarepool;

	// Массивы квадратов
	struct Level
	{
		int level;
		// Границы
		intpoint sqmin, sqmax;
		// Массив квадратов (указателей) размерность X*Z, ссылается на squarepool
		ed::vector< int32_t> pSquares;

		void Init(
		    intpoint sqmin, intpoint sqmax);

		lSuperficial* getSquare(intpoint sqindex, ed::vector<lSuperficial>& squarepool)
		{
			if(sqindex.x<sqmin.x || sqindex.x>=sqmax.x)
				return 0;
			if(sqindex.z<sqmin.z || sqindex.z>=sqmax.z)
				return 0;
			int i = (sqindex.x-sqmin.x)*(sqmax.z-sqmin.z) + (sqindex.z-sqmin.z);
			int32_t index = pSquares[i];
			if(index<0)
				return 0;
			if(index>=(int32_t)squarepool.size())
				return 0;
			return &squarepool[index];
		}

		lSuperficial* getSquare(const osg::Vec2i& squareIndex, ed::vector<lSuperficial>& squarepool)
		{
			if(squareIndex.x() < sqmin.x || squareIndex.x() >= sqmax.x)
				return NULL;
			if(squareIndex.y() < sqmin.z || squareIndex.y() >= sqmax.z)
				return NULL;

			int i = (squareIndex.x() - sqmin.x) * (sqmax.z - sqmin.z) + (squareIndex.y() - sqmin.z);
			int32_t index = pSquares[i];

			if(index < 0 || index >= (int32_t)squarepool.size())
				return NULL;

			return &squarepool[index];
		}
	};
	ed::vector< Level> pLevels;
	// Проверка семантики
	struct SemanticTest
	{
		// Имя семантики
		ed::string semantic;
		int semanticint;
		// Границы
		intpoint sqmin, sqmax;
		// Массив квадратов (указателей) размерность X*Z
		ed::vector< lSemanticSquare> squarepool;
		ed::vector< int32_t > pSquares;

		void Init(
		    const char* semname,
		    intpoint sqmin, intpoint sqmax);

		lSemanticSquare* getSquare(intpoint sqindex)
		{
			if(sqindex.x<sqmin.x || sqindex.x>=sqmax.x)
				return 0;
			if(sqindex.z<sqmin.z || sqindex.z>=sqmax.z)
				return 0;
			int i = (sqindex.x-sqmin.x)*(sqmax.z-sqmin.z) + (sqindex.z-sqmin.z);
			uint32_t index = pSquares[i];
			if(index>=squarepool.size())
				return 0;
			return &squarepool[index];
		}
	};
	ed::vector< SemanticTest> pSemanticTest;

	// Функции доступа
public:

#if defined(_DEBUG) || defined(DEBUG)
	ed::string debugFilename; // полный путь к sup3 файлу из которого был загружен этот lSuperficialFile
#endif

	EDTERRAIN_API bool Load(io::MmfStream& mmfStream);
	EDTERRAIN_API bool Save(const char* filename);
	bool Serialize(io::Stream& file);

	EDTERRAIN_API void InitAfterLoad();

	// Запрос по квадрату
	lSuperficial* getSuperficial(int level, intpoint square)
	{
		if(level>=(int)pLevels.size())
			return NULL;
		Level& lv = pLevels[level];
		return lv.getSquare(square, squarepool);
	}

	lSuperficial* getSuperficial(const osg::Vec2i& squareIndex, int level)
	{
		if(level < 0 || level >= (int)pLevels.size())
			return NULL;

		Level& lv = pLevels[level];
		return lv.getSquare(squareIndex, squarepool);
	}

	// Проверка семантики точки
	EDTERRAIN_API bool IsSemantic(int semantic, const dVector& pt, float* depth);
	EDTERRAIN_API bool IsSemantic(int semantic, const dVector& pt);
	EDTERRAIN_API bool IsSemantic(int semantic, const dVector& pt, geometry3::GeometryItem*& geom,
	                              unsigned long& faceIndex, dVector& sqPos);
#ifndef EDGE
	bool IsSemantic(int semantic, const osg::Vec3d& pt)
	{
		return IsSemantic(semantic, *(const dVector*)&pt);
	}
	bool IsSemantic(int semantic, const osg::Vec3d& pt, float* depth)
	{
		return IsSemantic(semantic, *(const dVector*)&pt, depth);
	}
	bool IsSemantic(int semantic, const osg::Vec3d& pt, geometry3::GeometryItem*& geom,
	                              unsigned long& faceIndex, osg::Vec3d& sqPos)
	{
		return IsSemantic(semantic, *(const dVector*)&pt, geom, faceIndex, *(dVector*)&sqPos);
	}
#endif
	// Все семантики в точке
	EDTERRAIN_API int GetSemantics(const dVector& pt, char** sems, int maxcount);

	// список геометрий в точке
	EDTERRAIN_API void getGeometryAtPoint(const osg::Vec3d& pt, ed::list<geometry3::GeometryItem*>& geoms);

	// есть ли сетка в точке
	EDTERRAIN_API bool isGeometryAtPoint(const osg::Vec3d& pt);
	// Функции построения
public:
	EDTERRAIN_API lSuperficialFile();
};
}

//#include "landscape3/lSuperficial.inl"

#endif