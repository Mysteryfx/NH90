#ifndef LANDSCAPE4_lCloudType_H
#define LANDSCAPE4_lCloudType_H

#include "edterrain.h"
#include "osg/Vec3d"
#include "osg/Vec3f"
#include "osg/Vec2f"
#include "osg/BoundingBox"
#include "osg/Matrixd"
#include <ed/string.h>
#include "ed/vector.h"
#include "ed/map.h"

namespace offshore
{
	struct Scene;
}


namespace landscape4
{
struct lCloudType
{
	struct ParticleDecl 
	{
		osg::Vec3f P;
		float R;
		osg::Vec2f UVmin;
		osg::Vec2f UVmax;

		osg::Vec2f UVgradient;		// UV для текстуры градиента
		float rnd;
	};
	struct MaterialDesc 
	{
		ed::string name;
		ed::string gradient;
		ed::string alpha;
		ed::string normals;
	};

	// Материал
	MaterialDesc material;

	// Частицы
	ed::vector< ParticleDecl> particles;

	// BB
	osg::BoundingBoxf box;

	// calcBoundingBox
	EDTERRAIN_API osg::BoundingBoxf calcBoundingBox(const osg::Matrixd& m);
};


// Загрузить тип облака из offshore::Scene
EDTERRAIN_API bool loadCloudFromOffshore( offshore::Scene& scene, int root, lCloudType& cloudtype);

// Загрузить все типы из offshore файла
EDTERRAIN_API bool loadCloudsFromOffshore( const ed::string& filename, ed::map<ed::string, lCloudType>& cloudtype);

};

#endif