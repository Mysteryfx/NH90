#ifndef RegStorage_H
#define RegStorage_H
#include "edObjects.h"

#include "Common.h"
#include "Serializer\Serializable.h"

#include "Registry/Registered.h"

// базовый класс для специализированных хранилищ
class EDOBJECTS_API RegStorage : public Common::Unknown<Serializable>
{
protected:
	ObjectID minID, maxID;
	viFlag types;

public:
	virtual ~RegStorage() {};

	inline viFlag CanContain() const	{return types;}
	ObjectID GetMinID() const			{return minID;}
	ObjectID GetMaxID() const			{return maxID;}

	virtual ObjectID Insert(Registered *obj) = 0;
	virtual bool Remove(Registered *obj) = 0;

	virtual ObjectID ForceID(Registered *obj, ObjectID id) = 0;

	virtual Registered *Get(ObjectID id) = 0;

	virtual Registered *Next(ObjectID id, viFlag oftype = viUnknown) = 0;
	virtual Registered *Prev(ObjectID id, viFlag oftype = viUnknown) = 0;

	virtual int DeleteAll(viFlag mask) = 0;

	virtual int Count() const = 0;

	virtual void serializeSelf(Serializer &) = 0;
	virtual void serializeObjects(Serializer &) = 0;

};

#endif // RegStorage_H