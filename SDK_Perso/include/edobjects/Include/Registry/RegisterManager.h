#ifndef RegisterManager_H
#define RegisterManager_H

#include "edObjects.h"

#include "Serializer\Serializable.h"

#include "Registered.h"

extern EDOBJECTS_API viFlag viDefaultSerializerMask;

class RegStorage;

class EDOBJECTS_API RegisterManager : public Common::Unknown<Serializable>
{
public:
	static RegisterManager *instance;

	// конструктор
	static RegisterManager *Create();
	virtual ~RegisterManager() {};

	virtual Registered *Get(ObjectID id) = 0;

	virtual Registered *Next(ObjectID id, viFlag oftype = viUnknown) = 0;
	virtual Registered *Prev(ObjectID id, viFlag oftype = viUnknown) = 0;

	virtual int DeleteAll(viFlag mask) = 0;

	// получить специализированное хранилище по типу
	virtual RegStorage *GetStorage(viFlag type) = 0;

	// новое хранилище
	virtual RegStorage* AddStorage(RegStorage *storage)=0;
	virtual bool DelStorage(RegStorage *storage)=0;

	// установить, какие объекты сериализуем
	virtual void SetSerializerMask(viFlag mask) = 0;
};

#endif // RegisterManager_H
