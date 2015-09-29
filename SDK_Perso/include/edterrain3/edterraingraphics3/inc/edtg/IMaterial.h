#ifndef EDTG_IMATERIAL_H
#define EDTG_IMATERIAL_H

#include "edtg.h"
#include "edtg/RenderItem.h"
#include "edtg/IGeometry.h"

namespace edtg
{
struct IMaterial;
struct IGeometry;
struct RenderContext;
struct TerrainOptions;

// реализация запрашивает потоки и параметры из геометрии
// Биндит их к своему шейдеру,
// Создает свою реализацию RenderItem
// Рендерит свои RenderItemы
struct IMaterial
{
public:
	virtual		~IMaterial(){}
	//========================================================================
	// init
	//    Инициализация инстанса данного материала для данного набора параметров
	//========================================================================
	virtual bool init(int argc, ed::string* argv) = 0;

	//========================================================================
	// init
	//    Инициализация инстанса данного материала для данного набора параметров
	//========================================================================
	virtual bool init(int argc, ed::string* argv, edtg::TerrainOptions* terrainOptions)
	{
		// Если не перегружен init с terrainOptions, дергаем старый вариант
		return this->init(argc, argv);
	}

	//========================================================================
	// bindGeometry
	//    реализация запрашивает потоки и параметры из геометрии
	//    создает и заполняет свою реализацию RenderItem
	//    materialString - строка декларации материала
	//    geometryString - строка декларации геометрии
	//    geometry       - геометрия
	//    terrainOptions - настройки земли
	//========================================================================
	virtual edtg::RenderItem* bindGeometry(const char* materialString, const char* geometryString,
		edtg::Geometry& geometry, edtg::TerrainOptions* terrainOptions, ed::string& error) = 0;

	//========================================================================
	// needRender
	//    Нужно ли рендерить рендер айтем data данным материалом. Например
	//    дополнительная проверка видимости и т.д.
	//    renderContext  - всевозможные матрицы для рендера
	//    terrainOptions - настройки земли
	//========================================================================
	virtual bool needRender(edtg::RenderItem* data, edtg::RenderContext* renderContext, edtg::TerrainOptions* terrainOptions)
	{
		return true;
	}

	//========================================================================
	// render
	//    Отрендерить data данным материалом. Рендер айтем должен был
	//    быть создан при помощи вызова bindGeometry у этого материала.
	//    renderContext  - всевозможные матрицы для рендера
	//    terrainOptions - настройки земли
	//========================================================================
	virtual void render(edtg::RenderItem* data, edtg::RenderContext* renderContext, edtg::TerrainOptions* terrainOptions) = 0;

	//========================================================================
	// dump
	//    Дамп рендер айтемов данного материала. FOR DEBUG ONLY
	//========================================================================
	virtual void dump(edtg::RenderItem* data, FILE* out, RenderContext* renderContext, edtg::TerrainOptions* terrainOptions) = 0;
};
}


#endif