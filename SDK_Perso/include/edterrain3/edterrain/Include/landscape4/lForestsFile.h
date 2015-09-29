#ifndef LANDSCAPE4_lForestsFile_H
#define LANDSCAPE4_lForestsFile_H

#include "edterrain.h"
#include "landscape3/lMaterial.h"
#include "landscape4/lDataFile.h"
#include "landscape4/lGeometry.h"
#include "landscape4/SpatialTree.h"
#include "landscape4/lRoadDetailsFile.h"
#include "osg/BoundingBoxOctree.h"

// lForestsFile.cpp
namespace landscape4
{

// 
class lForestsFile : public lRoadDetailsFile
{
public:
	EDTERRAIN_API lForestsFile();
	EDTERRAIN_API virtual ~lForestsFile();

	EDTERRAIN_API virtual ed::string getType() { return "landscape4::lForestsFile"; }

protected:
	bool serialize(io::Stream& s);
};

}
#endif