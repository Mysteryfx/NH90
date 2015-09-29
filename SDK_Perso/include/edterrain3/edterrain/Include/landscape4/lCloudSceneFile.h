#ifndef LANDSCAPE4_lCloudSceneFile_H
#define LANDSCAPE4_lCloudSceneFile_H

#include "edterrain.h"
#include "landscape4/lDataFile.h"
#include "landscape4/SpatialTree.h"

namespace landscape4
{

class lCloudSceneFile : public lDataFile
{
public:
	// Позиции облаков
	struct CloudInstance
	{
		// type см. lCloudSceneFile::cloudTypes
		int cloudType;

		// Позиция
		osg::Vec3d pos;
		osg::Quatd rot;
		float radius;

		// box
		osg::BoundingBoxf box;

		CloudInstance(){};

		EDTERRAIN_API osg::Matrixd getMatrix() const;
	};
	// Все облака
	ed::vector<CloudInstance> clouds;

	// Имена типов облакоы (на этот массив указывают индексы CloudInstance.cloudType)
	ed::vector<ed::string> cloudTypes;

	// spatial облаков
	landscape4::SpatialTree<uint32_t> spatialTree;

public:
	EDTERRAIN_API lCloudSceneFile();
	EDTERRAIN_API virtual ~lCloudSceneFile();

	virtual ed::string getType() { return "landscape4::lCloudSceneFile"; }
	virtual bool copyFrom(lDataFile* src){*this = *(lCloudSceneFile*)src;return true;};

	EDTERRAIN_API bool save(const char* filename);
	EDTERRAIN_API bool save(const ed::string& filename){return save(filename.c_str());};
	EDTERRAIN_API bool load(io::MmfStream& stream);

private:
	bool serialize(io::MmfStream& stream);
};

};

#endif