#pragma once
#include "edObjects.h"

#include "LandObjectStorage/lRealSceneFile.h"
#include "LandObjectStorage/BinarySceneAndTree.h"

#include "RegMapStorage.h"
#include "landscape3/lSceneFile.h"

// хранилища для наземных объектов с отложенной загрузкой
class EDOBJECTS_API RegLandObjectStorage4 : public RegMapStorage
{
public:
	RegLandObjectStorage4(ObjectID _minID, ObjectID _maxID);
	~RegLandObjectStorage4();
	
	virtual ObjectID Insert(Registered *obj);

	virtual Registered *Get(ObjectID id);
	Registered* createInstane(ObjectID id);

	// загрузить объект в параллельном потоке (сигнал поступает заранее)
	void LoadInTime(ObjectID id);
	
	// данные о статических объектах не сериализуются
	virtual void serialize(Serializer &) {};
};