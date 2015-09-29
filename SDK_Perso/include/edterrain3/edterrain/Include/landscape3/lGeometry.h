#ifndef __LANDSCAPE3_LGEOMETRY_H
#define __LANDSCAPE3_LGEOMETRY_H
#include "../landscape3_config.h"

#include "edterrain.h"
#include "lTerraBasic.h"

#include "landscape3/lMaterial.h"
#include <ed/vector.h>
#include <ed/list.h>

namespace offshore
{
struct Mesh;
}

namespace geometry3
{
// Макс. число текст. каналов
#define TV_MAX_CHANNEL 6

// Bound
struct BoundData
{
	Box box;
	Vector3 center;
	float radius;
};

struct GeometrySource;
struct GeometryItem;
typedef ed::list<GeometryItem*> tag_GeometryItemList;
typedef ed::list<GeometrySource*> tag_GeometrySourceList;

#ifndef NOOLDMAP
// Список сохраненных GeometrySource
//	typedef ed::map<GeometrySource*, GeometrySource*> tag_saved_geometrysource;
#endif

// Геометрия - элемент
struct GeometryItem
{
	// На источник
	GeometrySource* source;
	// Материал
	material3::Material* material;
	uint32_t materialindex_infile;			// для сохранения в файл
	// Начальное смещение
	uint32_t startFace;
	uint32_t startVert;
	// Число
	uint32_t nFaces;
	uint32_t nVerts;
	// Bound
	BoundData bound;
	// Приоритет отрисовки (слой)
	int32_t prior;

	/*/
	// текстурные массивы
	struct SubItem
	{
		// индекс тектуры в material (texturearray)
		int textureindex;
		// Начальное смещение
		uint32_t startFace;
		uint32_t startVert;
		// Число
		uint32_t nFaces;
		uint32_t nVerts;
	};
	ed::vector<SubItem> subitems;
	/*/
	// Функции
public:
	EDTERRAIN_API GeometryItem();
	EDTERRAIN_API GeometryItem(
	    GeometrySource* source,
	    unsigned int startFace,
	    unsigned int startVert,
	    unsigned int nFaces,
	    unsigned int nVerts,
	    material3::Material* material=NULL);
	// Копировать геометрию
	void CopyFrom(GeometryItem& src);
	// Копировать описание геометрии
	void CopyFrom(GeometryItem& src, int vertoffset, int faceoffset);
	// Копировать источник геометрии
	void CopySource(GeometryItem& src, int nFaces, int nVerts);
	// Насчитать бокс
	EDTERRAIN_API void BuildBound();
	// Проверка попадания точки
	bool IsPointInside(float x, float z, float* depth);
	bool IsPointInside(float x, float z);
#ifndef EDGE
	bool IsPointInside(Vector3& pt)
	{
		return IsPointInside(pt.x, pt.z);
	}
#endif
	//Версия IsPointInside сохраняющая треугольник в котором находится точка
	bool IsPointInside(float x, float z, unsigned long& faceIndex);

	bool operator ==(const GeometryItem& arg) const
	{
		if(source!=arg.source) return false;
		return match_nosource(arg);
	}
	bool match_nosource(const GeometryItem& arg) const
	{
		if(startFace!=arg.startFace) return false;
		if(startVert!=arg.startVert) return false;
		if(nFaces!=arg.nFaces) return false;
		if(nVerts!=arg.nVerts) return false;
		if(prior != arg.prior) return false;

		if(*material != *arg.material) return false;

		return true;
	}
	bool operator !=(const GeometryItem& arg) const
	{
		return !(*this==arg);
	}

	// Обработать каждый вертекс
	typedef void (*tag_ProcessVertex)(lv3V& vert, uint32_t data);

	void foreach_vertex(tag_ProcessVertex proc, uint32_t data);
	void preload();

	// сервисные ф-ции
public:
	EDTERRAIN_API void toOffshoreMesh(
	    offshore::Mesh& mesh
	);

};

// Геометрия - источник
struct GeometrySource
{
	// Геометрия
	io::mmfvector<li3V>  faces;		// или faces
	io::mmfvector<li2V>	 lines;		// или lines
	io::mmfvector<lv3V>  verts;

	io::mmfvector<ln3V>  tangents;
	io::mmfvector<ln3V>  binormals;
	io::mmfvector<ln3V>  norms;

	io::mmfvector<ltv2V> tverts[TV_MAX_CHANNEL];
	io::mmfvector<uint32_t> colors;
	// индекс текстуры, для использования текстурного массива
	io::mmfvector<uint8_t> textureindex;

	// Функции
public:
	EDTERRAIN_API GeometrySource();
	EDTERRAIN_API ~GeometrySource();	// Вызывается только в памяти
	EDTERRAIN_API GeometrySource(const GeometrySource& src);
	EDTERRAIN_API GeometrySource(const GeometryItem& src);

	bool isEmpty()const
	{
		return !(!faces.empty() || !lines.empty());
	};

	EDTERRAIN_API void Init(const GeometrySource& src);
	EDTERRAIN_API void Init(const GeometryItem& src);

	// Поверхность?
	bool IsTriangleList()
	{
		return !faces.empty();
	}
	// Линия?
	bool IsLineList()
	{
		return !lines.empty();
	}
	// Тип (как DX)
	EDTERRAIN_API uint32_t getType();
	// Число каналов
	int  numChanels()
	{
		int t=0;
		for(; t<TV_MAX_CHANNEL; t++)
			if(tverts[t].empty()) break;
		return t;
	}
	// Проверка попадания точки
	EDTERRAIN_API bool IsPointInside(float x, float z, int startface, int faces);

	// Ф-ции создания
	EDTERRAIN_API void setFaces(li3V* src, int count);
	EDTERRAIN_API void setLines(li2V* src, int count);
	EDTERRAIN_API void setVerts(lv3V* src, int count);
	EDTERRAIN_API void setNorms(ln3V* src, int count);
	EDTERRAIN_API void setTangents(ln3V* src, int count);
	EDTERRAIN_API void setBiNormals(ln3V* src, int count);
	EDTERRAIN_API void setTVerts(int chenal, ltv2V* src, int count);

	EDTERRAIN_API void BuildFromList(
	    ed::list<GeometryItem*>& src,
	    ed::vector< GeometryItem >& geomlist
	);

	// Bound
	EDTERRAIN_API void BuildBound(BoundData& bound);

	// Сравнить
	bool match(GeometrySource& arg);

	// Создать источник такого же типа
	void CreateSame(GeometrySource& src, int nVerts, int nFaces);

	// Копирование по частям
	void Copy_Step1(GeometrySource& src);
	void Copy_Step2(GeometrySource& src);
	void Copy_Step3(GeometrySource& src);

	enum PrimiriveType
	{
		FACES = 1,
		LINES = 2,
	};

	EDTERRAIN_API int getPrimitiveType();

public:
	EDTERRAIN_API void preload();
	EDTERRAIN_API void discard();

#ifndef NOOLDMAP
	// Собрать геометрии и источники в кучу
	static void Assemble(
	    tag_GeometryItemList& src_list,
	    //			material3::tag_saved_materials& savedmaterials,
	    //			geometry3::tag_saved_geometrysource& saved_geometrysource,
	    tag_GeometryItemList& dst_gi_list,
	    tag_GeometrySourceList& dst_gs_list
	);
#endif
protected:
	// Переиндексация тр-ков
	void Reindex(int offset);
};


/*/
void AddToGeometrySourceSavedList(
	GeometrySource* gs,
	tag_saved_geometrysource& saved_geometrysource);
void AddToGeometrySourceSavedList(
	tag_GeometrySourceList& dst_gs_list,
	tag_saved_geometrysource& saved_geometrysource);
/*/

#ifndef NOOLDMAP

#endif

};

//#include "landscape3/lGeometry.inl"
#endif
