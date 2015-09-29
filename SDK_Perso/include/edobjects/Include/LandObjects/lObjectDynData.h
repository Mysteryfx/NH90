#ifndef lObjectDynData_H
#define lObjectDynData_H

#include "Common.h"
#include "Serializer\Serializable.h"

#include "lLandObject.h"

namespace Mail { class Message; }

#define IID_lObjectDynData Common::identify<lObjectDynData>()

// динамические данные о наземном объекте
class EDOBJECTS_API lObjectDynData : public Common::FakeUnknown<Serializable>
{
private:
	lLandObject *me;
	double life;

	cPosition pos0;
	wShape *type0;

	// информация о состоянии камеры объекта
	// будет удалено при удалении дин. информации
	Suicide *camera_context;

public:
	lObjectDynData();
	virtual ~lObjectDynData();

	// Пришлось декларировать иначе под интелом не линкуется.Mitka
	lObjectDynData(const lObjectDynData& arg);
	lObjectDynData& operator =(const lObjectDynData& arg);

	virtual void Init(lLandObject *owner);
	void SetOwner(lLandObject *owner);

	inline lLandObject *Me(){return me;}
	inline double Life()		{return life;}
	inline wShape *Type0()	{return type0;}
	inline cPosition &Pos0(){return pos0;}

	Suicide *GetCameraContext();
	void SetCameraContext(Suicide *context);

	virtual void SetLife(double _life);

	virtual void serialize(Serializer &);
};

#endif // lObjectDynData_H