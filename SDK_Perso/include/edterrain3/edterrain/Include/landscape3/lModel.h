#ifndef __LANDSCAPE3_LMODEL_H
#define __LANDSCAPE3_LMODEL_H
#include "../landscape3_config.h"

#include "edTerrain.h"

#include "landscape3/lMaterial.h"
#include "landscape3/lGeometry.h"

#include "io/File.h"

namespace models3
{
class lModel
{
public:
	ed::vector< geometry3::GeometrySource> geomsource;
	// Геометрии
	ed::vector< geometry3::GeometryItem> geoms;
	// индексы к какой GeometrySource относится GeometryItem
	ed::vector< uint32_t> geomsource_inds;

	// Инициализация после загрузки
	void InitAfterLoad(
	    material3::Material* materials	// Материалы
	);
};
// Именованная модель
struct ModelKey
{
	// Имя модели
	ed::string name;
	// Элементы
	lModel model;
};

// Файл моделей
struct File
{
	// Материалы
	ed::vector< material3::Material> materials;
	// Сортированный список моделей
	ed::vector< ModelKey > models;

	// Функции доступа
public:
	// Открыть файл
	EDTERRAIN_API bool Load(
	    io::MmfStream& mmfStream
	);
	EDTERRAIN_API bool Save(
	    const char* filename
	);
	bool Serialize(io::Stream& file);

	// Модель по имени
	EDTERRAIN_API lModel* getModel(const char* name);
	// Модели
	EDTERRAIN_API int getModelCount();
	EDTERRAIN_API lModel* getModel(int index);

public:
	EDTERRAIN_API File();
};
}

//#include "landscape3/lModel.inl"
#endif