#ifndef ED_MODEL_VIS_INSTANCE_PARAMS_H
#define ED_MODEL_VIS_INSTANCE_PARAMS_H

#include "osg/Matrixd"
#include "ed/vector.h"

#include "Arguments.h"

namespace model
{

class IModelParams;

struct FlatShadowInfo{
	/// Coefficients for projection plane
	osg::Vec4f plane;
	/// x - shadow strength, y - terrain height.
	osg::Vec2f props;

	FlatShadowInfo(const osg::Vec3d& p, const osg::Vec3f& n,float shadowStrength):
		plane(n.x(), n.y(), n.z(), -p * n),
		props(shadowStrength, p.y() - 0.01){
	}
};

struct InstanceParams
{
	const IModelParams &modelParams;
	osg::Matrixd* transformMatrices;
	unsigned int nMatrices;
	const FlatShadowInfo *flatShadowInfo;

	InstanceParams(const IModelParams &modelParams):modelParams(modelParams), transformMatrices(NULL), nMatrices(0), flatShadowInfo(NULL){}

	inline const Arguments& getArguments()const{
		return modelParams.getArgs();
	}

	inline const ed::vector<osg::Matrixd>& getBones()const{
		return modelParams.getBones();
	}
};

}
#endif
