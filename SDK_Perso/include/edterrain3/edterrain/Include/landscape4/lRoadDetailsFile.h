#ifndef LANDSCAPE4_lRoadDetailsFile_H
#define LANDSCAPE4_lRoadDetailsFile_H

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lDataFile.h"
#include "landscape4/lGeometry.h"
#include "landscape4/SpatialTree.h"

// lRoadDetailsFile.cpp
namespace landscape4
{

// 
class lRoadDetailsFile : public lDataFile
{
public:
	static uint32_t getMaxLevel( uint32_t levelmask);

	// куски геометрий, фрагменты
	struct Fragment : public geometry4::GeometryItem
	{
		int geometryIndex;	// в массиве geometries
		int objectIndex;	// в массиве objects
		osg::BoundingBox box; // box
		osg::Vec3f pos;	// позиция
	};

	// Инстансируемый объект. состоит из нескольких кусков/лодов
	struct Object
	{
		ed::string name;
		osg::BoundingBox fullbox;
		int maxlevel;
		bool bHwInstancing;

		struct Lod
		{
			ed::string name;
			uint32_t levelmask;					// маска level
			osg::BoundingBox box;
			material3::Material material;
			geometry4::GeometrySource geometry;
		};
		ed::vector<Lod> lods;

		// Фрагменты
		io::mmfvector<Fragment> fragments;

		Object():bHwInstancing(0){};
	};

	// объекты для инстансирования
	ed::vector<Object> objects;

	// геометрии инстансеров
	ed::vector<geometry4::GeometrySource> geometries;

	// Пара: индекс объекта и индекс фрагмента в Object.fragments
	typedef std::pair<uint32_t, uint32_t> FragmentRef;
	// spatial для каждого уровня
	struct Level
	{
		// spatial. хранит ссылки на фрагменты
		landscape4::SpatialTree< FragmentRef> spatial;
	};
	ed::vector<Level> levels;

public:
	EDTERRAIN_API lRoadDetailsFile();
	EDTERRAIN_API virtual ~lRoadDetailsFile();

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool load(const char* filename);
	EDTERRAIN_API bool load(io::MmfStream& file);

	EDTERRAIN_API virtual ed::string getType() { return "landscape4::lRoadDetailsFile"; }

	EDTERRAIN_API virtual bool copyFrom(lDataFile* src){*this = *(lRoadDetailsFile*)src;return true;};

protected:
	virtual bool serialize(io::Stream& s);
};

inline uint32_t lRoadDetailsFile::getMaxLevel( uint32_t levelmask)
{
	uint32_t maxlevel=0;
	for( ; levelmask; maxlevel++)
		levelmask = levelmask>>1;
	return maxlevel-1;
}

}




#endif