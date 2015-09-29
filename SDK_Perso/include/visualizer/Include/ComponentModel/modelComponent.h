#pragma once
#include "Resourcer/ILiveryManager.h"
#include "Modeler/ModelInstance.h"
#include "Modeler/Model.h"
#include "entity.h"

class ModelComponent: public EntityComponent, Graphics::ModelInstance
{
private:
	unsigned int livery;
public:
	Graphics::Model* model;
	Graphics::DParamList params;
	Position3 *pos;

	ModelComponent(Graphics::Model* model=NULL);
	virtual bool init();
	virtual unsigned int getLiveryHandle() const { return livery; }
	virtual void setLiveryHandle(unsigned int liveryHandle);
	// �������� ������� ����������
	virtual const Position3& GetPosition(){return *pos;}
	// �������� ������ ���������� ���������
	virtual const Graphics::DParamList& GetDrawArguments(){return params;}
	// �������� ������
	virtual Graphics::Model *GetModel(){return model;}

	inline Graphics::ModelInstance *asModelInstance() {return this;}
};

