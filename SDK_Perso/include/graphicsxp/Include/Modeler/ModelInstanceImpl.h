#ifndef ModelInstanceImpl_H
#define ModelInstanceImpl_H

#include "Modeler/ModelInstance.h"
#include "Modeler/Model.h"
#include "graphicsxp.h"

namespace Graphics
{

// Простейшая реализация ModelInstance
class ModelInstanceImpl : public ModelInstance{
private:
	unsigned int _livery;
public:
	Model* model;
	DParamList params;
	Position3 pos;
public:
	GRAPHICSXP_API ModelInstanceImpl(Model* model=NULL);
	
	virtual unsigned int getLiveryHandle()const{return _livery;}
	GRAPHICSXP_API virtual void setLiveryHandle(unsigned int liveryHandle);

	// получить текущие координаты
	virtual const Position3& GetPosition(){return pos;}
	// получить список аргументов рисования
	virtual const DParamList& GetDrawArguments(){return params;}
	// получить модель
	virtual Model *GetModel(){return model;}
};

}

#endif
