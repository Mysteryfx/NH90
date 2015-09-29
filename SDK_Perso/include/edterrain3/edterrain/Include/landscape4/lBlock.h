#ifndef LANDSCAPE4_LBLOCK_H
#define LANDSCAPE4_LBLOCK_H

#include "../landscape3_config.h"
#include <osg/Matrixd>
#include "../landscape3/lMaterial.h"
#include "../landscape4/lGeometry.h"
#include "landscape4/lDataFile.h"
#include "landscape4/SpatialTree.h"

#ifdef EDGE
namespace model
{
	class IGraphModel;
}
#endif

namespace scene4
{

// Геометрия одного квартала
struct lBlock
{
	// Название квартала
	ed::string name;

	// Информация о костях квартала, необходимая при создании сцены кварталов
	struct Joint
	{
		// Имя домика привязанного к этой кости. Для рендера edm-ок
		ed::string modelName;

		// Баунд кости
		osg::BoundingBoxf box;

		// Текстурная координата кости
		osg::Vec2d uv;

		// x, z - 2 базисных вектора в пространстве текстурных координат
		osg::Vec2d x;
		osg::Vec2d z;
	};

	ed::vector<Joint> joints;

	// геометрии кваратала: лоды домиков, деревья, лайты ...
	struct Geometry : public geometry4::GeometrySource
	{
		ed::string name;

		// индекс в массиве geometries
		uint32_t index;

		// материал
		material3::Material material;
	};

	ed::vector<Geometry> geometries;

	// ...
	float distanceFactor;

	enum enFlags
	{
		OUT_OF_AREA = 1,		// Рисуется только за границей uv диапазона 0-1
		RANDOM_ROTATE = 2,		// Рисуется только за границей uv диапазона 0-1
	};
	struct Detail
	{
		ed::string name;
		ed::vector<osg::Vec2d> uv;
		ed::vector<osg::Vec2d> x;
		ed::vector<uint8_t> flags;		// enFlags
	};
	ed::vector<Detail> details;


	EDTERRAIN_API bool load(io::MmfStream& stream);
	EDTERRAIN_API bool load(const char* filename);
	EDTERRAIN_API bool load(const ed::string& filename);

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename);
};

// Инстанс одного квартала в сцене с кварталами.
// В этой структуре хранятся неизменяемые данные этого инстанса квартала
struct lBlockInstance
{
	enum Flags
	{
		VALID = 1, // Домик валидный и его надо рендерить
	};

	// тип квартала. индекс в массиве BlockSceneFile.blockNames
	uint16_t blockType;

	// баунд данного инстанса квартала (в World Space)
	osg::BoundingBoxf boxWS;

	// позиции домов квартала (в World Space)
	io::mmfvector<osg::Matrixf> objectPos;

	// тип домика. индекс в массиве BlockSceneFile.buildingNames
	io::mmfvector<uint16_t> objectType;

	// битовая маска валидности для каждой геометрии квартала
	io::mmfvector<uint8_t> objectFlag;

	// y компоненты векторов objectPos[].x и objectPos[].z
	// Такие чтоб cross(objectPos[].x, objectPos[].z) давал нормаль поверхности
	// Нужно для локоновских теней
	io::mmfvector<osg::Vec3f> surfaceSlope;
};

// Все изменяемые данные для одного инстанса квартала
struct lBlockInstanceDynamic
{
	lBlockInstance* blockInstance;

	// Центр баунда этого инстанса квартала (в World Space)
	osg::Vec3f center;
	float radius;

	// Удаленные домики квартала
	ed::vector<uint16_t> deletedBuildings;

	// Поинтер на какие-либо объекты необходимые для рендера (поинтер на константый буфер с костями)
	void* renderData;

	// ...
	float distanceFactor;
};

// Сцена с кварталами
struct lBlockSceneFile : public landscape4::lDataFile
{
	// Имена кварталов, используемых в этой сцене
	ed::vector<ed::string> blockNames;

	// Поинтеры на загруженные типы кварталов
	ed::vector<lBlock*> blocks;

	// Инстансы кварталов в сцене
	ed::vector<lBlockInstance> instances;
	ed::vector<lBlockInstanceDynamic> dynamicInstances;

	// Дерево кварталов. Хранит индексы на элементы массива instances
	landscape4::SpatialTree<uint32_t> spatialTree;

	// Имена квартальных домиков (на этот массив указывают индексы lBlockInstance.objectType)
	ed::vector<ed::string> buildingNames;

	// Боксы квартальных домиков (на этот массив указывают индексы lBlockInstance.objectType)
	ed::vector<osg::BoundingBoxf> buildingBoxes;

#ifdef EDGE
	// Поинтеры на загруженные модели домиков, соответствуют buildingNames
	ed::vector<model::IGraphModel*> buildings;

	// id референсов для рендера
	ed::vector<int> buildingReferences;
#else
	//ed::vector<wShape*> buildings;
#endif

	virtual ed::string getType() { return "scene4::BlockSceneFile"; }
	virtual bool copyFrom(lDataFile* src) { *this = *(lBlockSceneFile*)src; return true; }

	//...

	EDTERRAIN_API bool load(io::MmfStream& stream);

	EDTERRAIN_API bool load(const char* filename);
	EDTERRAIN_API bool load(const ed::string& filename);

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename);

// Запрос объектов
public:
	// Базовый класс для запросов к сцене
	class ISink
	{
	public:
		// blockIndex - индекс квартала, objectIndex - индекс объекта в квартале
		virtual void onQuery(uint32_t blockIndex, uint32_t objectIndex, const osg::Matrixd& pos, int32_t type) = 0;
	};

	/**
	 * Выбрать объекты пересекающие фрастум, баундинг бокс или отрезок
	 */
	EDTERRAIN_API void query(const osg::Polytope& frustum, ISink* sink) const;
	EDTERRAIN_API void query(const osg::BoundingBox& box, ISink* sink) const;
	EDTERRAIN_API void query(const osg::Vec3d& a, const osg::Vec3d& b, ISink* sink) const;

private:

	EDTERRAIN_API bool serialize(io::MmfStream& stream);
};

}


#endif
