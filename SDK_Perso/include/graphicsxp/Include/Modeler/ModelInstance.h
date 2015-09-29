#ifndef ModelInstance_H
#define ModelInstance_H

#include "GraphMath\position.h"
#include "Renderer\DynamicParams.h"
#include "graphicsxp.h"
#include "osg/Matrixd"
#include "ed/vector.h"
#include "edModel/ModelParams.h"

namespace Graphics
{

class Model;
class IModelParams;
class ModelConnector;
class ModelParser;

class ModelInstance;

namespace prv{

class NewModelParams : public model::IModelParams{
private:
	ModelInstance &mi;
	ed::vector<osg::Matrixd> bones;
public:
	GRAPHICSXP_API NewModelParams(ModelInstance &mi);
	
	GRAPHICSXP_API virtual void init(const model::IModel &model);

	GRAPHICSXP_API virtual model::Arguments& getArgs();
	GRAPHICSXP_API virtual const model::Arguments& getArgs()const;

	GRAPHICSXP_API virtual ed::vector<osg::Matrixd>& getBones();
	GRAPHICSXP_API virtual const ed::vector<osg::Matrixd>& getBones()const;	
};
}

// интерфейс для экземпляра модели (при взгляде изнутри модели)
class ModelInstance{
private:
	prv::NewModelParams modelParams;
public:
	GRAPHICSXP_API ModelInstance();
	GRAPHICSXP_API virtual ~ModelInstance();

	/// Sets livery handle of model.
	virtual void setLiveryHandle(unsigned int liveryHandle){}

	/// Returns livery handle, may return INVALID_LIVERY_HANDLE.
	GRAPHICSXP_API virtual unsigned int getLiveryHandle()const;

	// получить текущие координаты
	virtual const Position3& GetPosition() = 0;

	// получить список аргументов рисования
	virtual const DParamList& GetDrawArguments() = 0;

	GRAPHICSXP_API virtual const ed::vector<osg::Matrixd>& getBones()const;

	// получить модель
	virtual Model *GetModel() = 0;

	virtual const model::IModelParams* GetModelParams()const{return &modelParams;}
	virtual model::IModelParams* GetModelParams(){return &modelParams;}
};

} // namespace Graphics

#endif // ModelInstance_H
