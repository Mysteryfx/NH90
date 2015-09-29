#ifndef GRAPHICSXP_MODEL_PARAMS_IMPL_H
#define GRAPHICSXP_MODEL_PARAMS_IMPL_H

#include "edModel/Arguments.h"
#include "edModel/IModel.h"

namespace model{

class ModelParamsImpl : public IModelParams
{
private:
	/// Holds parameters of model which'll be sent to model.
	Arguments parameters;
	ed::vector<osg::Matrixd> bones;
public:
	virtual void init(const IModel &model){
		auto n = model.getNumBones();
		if(n > bones.size()){
			bones.resize(n);
		}
		
		n = model.getNumArguments();
		if(n > parameters.size()){
			parameters.resize(n);
		}
	}

	Arguments& getArgs(){return parameters;}
	virtual const Arguments& getArgs()const{return parameters;}

	ed::vector<osg::Matrixd>& getBones(){return bones;}
	virtual const ed::vector<osg::Matrixd>& getBones()const{return bones;}
};


}

#endif
