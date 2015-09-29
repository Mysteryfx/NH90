#ifndef RegLandObjectStorage_H
#define RegLandObjectStorage_H
#ifndef USE_TERRAIN4
#include "edObjects.h"

#include "LandObjectStorage/lRealSceneFile.h"
#include "LandObjectStorage/BinarySceneAndTree.h"

#include "RegVectorStorage.h"
#include "landscape3/lSceneFile.h"

// хранилища для наземных объектов с отложенной загрузкой
class EDOBJECTS_API RegLandObjectStorage : public RegVectorStorage
{
protected:
	ObjectID curID;

    struct scene_t
    {
        ObjectID startId;
        scene3::lSceneFile* scene;
    };
    ed::vector<scene_t> _scenefiles;

public:
	RegLandObjectStorage(ObjectID _minID, ObjectID _maxID);
	~RegLandObjectStorage();

	// Открыть
	bool Open(
		scene3::lSceneFile* _scenefile, 
		ObjectID startId
		);

    // Закрыть
    void Close(scene3::lSceneFile* _scenefile);

	virtual ObjectID Insert(Registered *obj);

	virtual Registered *Get(ObjectID id);

	// загрузить объект в параллельном потоке (сигнал поступает заранее)
	void LoadInTime(ObjectID id);
	
	// данные о статических объектах не сериализуются
	virtual void serialize(Serializer &) {};
};

#endif // ifndef USE_TERRAIN4
#endif // RegLandObjectStorage_H