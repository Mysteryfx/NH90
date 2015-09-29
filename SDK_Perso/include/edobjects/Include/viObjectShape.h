#ifndef viObjectShape_H
#define viObjectShape_H

#include "viObject.h"
#include "Modeler/ModelInstance.h"
#include "Modeler/gGeometry.h"
#include "viObjectHandle.h"

// предназначение получаемых аргументов
enum viArgumentPurpose
{
	viDraw,			// для рисования
	viCollision,	// для проверки столкновений
};


// Список аргументов рисования
struct viDrawArguments
{
	bool delete_after_use;
	float *args;
	int count;
};

class ILivery;

class ModelLights;

// Интерфейс для рисуемых объектов
class EDOBJECTS_API viObjectShape : public viObject, public Graphics::ModelInstance, public ISceneObject
{
protected:
	unsigned int _livery;
	Graphics::Model *model;
	ModelLights *lights;
	gBox lbox;
public:
	viObjectShape(viFlag _type = viUnknown);
	virtual ~viObjectShape();

	virtual void setLiveryHandle(unsigned int liveryHandle);

	virtual unsigned int getLiveryHandle()const;

	/// This method must set livery handle to INVALID_LIVERY_HANDLE.	
	virtual void SetModel(const ed::string &modelname);
	/// This method must set livery handle to INVALID_LIVERY_HANDLE.	
	virtual void SetModel(Graphics::Model *_model);
	
	const gBox& getLocalBox()const;

	// не требуют дальнейшего перекрытия
	virtual const gBox &LocalBox(wModelTime t);
	
	// интерфейс ModelInstance
	virtual const Graphics::DParamList& GetDrawArguments();
	virtual Graphics::Model *GetModel();

	// получить точную координату объекта
	virtual dVector GetPoint();
    inline virtual float getVisibility()const{return 1;};

	// Для присоединенных дымов. Создать такие объекты
	virtual void InitConnectedObjects(){};
	// парсинг (отрисовка, проверка столкновений и т.д.)
	// если заполнены индивидуальные эффекты на текущий кадр - они имеют приоритет
	virtual void Parse(
		Graphics::ModelParser& parser,
		const Position3& pos,
		Graphics::effectState* effects=0);


public:
	// получить индивидуальный список эффектов (TODO: сделать нормальный viEffect)
	Graphics::effectState &GetEffectState();

	// сигнал о изменении индивидуального состояния по эффектам
	// вернет true если это первый такой вызов в кадре (надо добавить глобальные эффекты)
	bool OnChangeEffectState();

public:
	//Scene ObjectInterface
	ObjectID	               getObjectID()       const override;
	osg::Matrixd               getObjectPosition() const override;
	model::IModel*             getObjectType()     const override;
	const model::IModelParams* getModelParams()    const override;

protected:
	// индивидуальное состояние по эффектам
	Graphics::effectState effects;
	unsigned effects_frame;
};


EDOBJECTS_API void setArgsFromLivery(ILivery			  * livery,
									 Graphics::DParamList & drawArgs,
									 int default_arg_for_scheme = -1);


#endif // viObjectShape_H
