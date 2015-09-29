#ifndef ED_MODEL_VIS_MODEL_PARAMS_H
#define ED_MODEL_VIS_MODEL_PARAMS_H

#include "osg/Matrixd"
#include "ed/vector.h"

#include "Arguments.h"

namespace model
{

class IModel;

class IModelParams{
public:
	virtual ~IModelParams(){}

	virtual void init(const IModel &model) = 0;

	virtual Arguments& getArgs() = 0;
	virtual const Arguments& getArgs()const = 0;

	virtual ed::vector<osg::Matrixd>& getBones() = 0;
	virtual const ed::vector<osg::Matrixd>& getBones()const = 0;
};

}
#endif
