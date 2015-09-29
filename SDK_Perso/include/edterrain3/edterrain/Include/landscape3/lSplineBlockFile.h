#ifndef __LANDSCAPE3_lSplineBlockFile_FILE_H
#define __LANDSCAPE3_lSplineBlockFile_FILE_H
#include "../landscape3_config.h"

#include <osg/Vec3d>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/BoundingBox>
#include "edterrain.h"
#include "landscape3/lMaterial.h"

//
// Файл протяженного объекта
// Используется для заборов, лесопосадок, столбов и прочих протяженных объектов расставляемых по B-сплайну
//
// В рендер передаются точки сплайна в WS
// Рендер расчитан что все константы влезают в один вызов
//
// Моделится из расчета ориентации по оси X
//
namespace scene3
{
//
class lSplineBlockFile
{
public:
	// Материал
	material3::Material material;

	io::mmfvector<osg::Vec3f> pos;			//
	io::mmfvector<osg::Vec3f> normal;		//
	io::mmfvector<osg::Vec2f> uv1;			//
	io::mmfvector<float> parameter;			// параметр сплайна (целая часть - номер отрезка)

	// индексы тр-ков (facecount*3)
	io::mmfvector<uint32_t> indecies;

public:
	EDTERRAIN_API lSplineBlockFile();
	EDTERRAIN_API ~lSplineBlockFile();

	// Загрузка
public:
	EDTERRAIN_API bool Load(io::MmfStream& mmfStream);
	EDTERRAIN_API bool Save(const char* filename);
	EDTERRAIN_API bool Serialize(io::Stream& file);
};

};

#endif
