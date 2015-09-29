#ifndef __LANDSCAPE3_LBLOCK_FILE_H
#define __LANDSCAPE3_LBLOCK_FILE_H
#include "../landscape3_config.h"

#include <osg/Vec3d>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/BoundingBox>
#include "edterrain.h"
#include "landscape3/lMaterial.h"

//
// Файл квартала
// Используется для рендера и расстановки домов по текстуре
//
namespace scene3
{

struct BlockBone
{
	// Имя модели
	ed::string modelname;
	// x, y - 2 базисных вектора в пространстве текстурных координат
	osg::Vec2d x;
	osg::Vec2d y;
	//Текстурная координата кости
	osg::Vec2d uv;
	//Bounding box модели
	osg::BoundingBox bBox;
};
// Файл квартала
// Требование: треугольники одного домика лежат в списке indecies рядом
// Иначе невозможно рендерить квартал в котором число домов превышает ограничения шейдера
//
class lBlockFile
{
public:

	ed::vector<BlockBone> bones;

	struct LODGeometry
	{
	public:
		LODGeometry()
		{
			isValid = true;
			material_index = 0;
		}

		//Vertex data
		io::mmfvector<osg::Vec3f> pos;
		io::mmfvector<osg::Vec3f> normal;
		io::mmfvector<osg::Vec2f> uv1;
		io::mmfvector<osg::Vec2f> uv2;
		io::mmfvector<float> bone;
		io::mmfvector<float> vertNumber;
		io::mmfvector<float> scale;
		io::mmfvector<float> yrotate;
		io::mmfvector<float> specFactor;
		io::mmfvector<float> specPower;
		io::mmfvector<float> specPowerAmount;
		io::mmfvector<float> selfIlluminationAmount;
		io::mmfvector<float> reflectionAmount;
		// индексы тр-ков (facecount*3)
		io::mmfvector<uint32_t> indecies;
		// начальный треугольник домика. см lBlockFile::indecies
		io::mmfvector<uint16_t> startFace;
		io::mmfvector<uint16_t> lastFace;
		// Направление источников света
		io::mmfvector<float> direction;
		// Тип источника 0 - не направленный, 1 - направленный
		io::mmfvector<float> typedirection;
		// веса костей для линейной интерполяции (может быть 0)
		io::mmfvector<float> weights;
		// цвета лампочек
		io::mmfvector<uint32_t> color;
		// материал
		int32_t material_index;
		// используется ли лод
		bool isValid;
		// Высота зданий
		io::mmfvector<float> height;
	};

	ed::map<ed::string, LODGeometry> lods;
	// Материалы
	ed::vector<material3::Material> materials;
	bool unique;
	double distanceFactor;

	// Ширина квартала(используется только в сплайновом квартале)
	double width;

public:
	EDTERRAIN_API lBlockFile();
	EDTERRAIN_API ~lBlockFile();

	// Загрузка
public:
	EDTERRAIN_API bool Load(io::MmfStream& mmfStream);
	EDTERRAIN_API bool Save(const char* filename);
	EDTERRAIN_API bool Serialize(io::Stream& file);
};

};

#endif
