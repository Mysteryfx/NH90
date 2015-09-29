#ifndef lCustomLandObject_H
#define lCustomLandObject_H

#include "viFlags.h"
#include "lLandObject.h"

#define IID_lCustomLandObject Common::identify<lCustomLandObject>()


// Наземный объект, с возможностью управления моделью и ее аргументами
class EDOBJECTS_API lCustomLandObject : public lLandObject
{
protected:
	Graphics::DParamList dparams;
	dVector				 pos_p;
public:
	static lCustomLandObject* CreateObject();
	static lCustomLandObject* CreateObject(Graphics::Model *SkinnedModel, const cPosition& _pos, wModelTime t = 0.0, viFlag status = viDynamicObject);
	static lCustomLandObject* CreateObject(const char *modelname, const cPosition& _pos, wModelTime t = 0.0, viFlag status = viDynamicObject);
	lCustomLandObject();
	~lCustomLandObject();

	virtual void Init(wShape *_type, const cPosition& _pos, wModelTime t, viFlag status = viUnknown, void* context=NULL);

	// управление параметрами
	virtual void SetModel(const ed::string &filename);
	virtual void SetModel(Graphics::Model *_model);
	void SetModelParam(int i, float v);
	void SetPoint(const dVector & point)
	{
		pos_p  = point;
	}

	dVector GetPoint()
	{
		return pos_p;
	}
	// доступ
	virtual Graphics::Model* GetModel();
	virtual const Graphics::DParamList& GetDrawArguments();

	// восстановить исходное состояние (если создавалась dyndata)
	virtual void ReinitState();

	void serialize(Serializer &);

	wtPointer		Kill;  //деятельность для часовой бомбы
	wModelTime      deadTime; //назначенное время смерти
};

class EDOBJECTS_API lCustomLandObjectActivity : public wTime
{
public:
	lCustomLandObjectActivity(lCustomLandObject *p, wModelTime dt);
	virtual ~lCustomLandObjectActivity();
	virtual void NextEvent();

	lCustomLandObject *p;
};

#endif // lCustomLandObject_H
