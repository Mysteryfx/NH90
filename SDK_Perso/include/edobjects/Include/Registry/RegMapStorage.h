#ifndef RegMapStorage_H
#define RegMapStorage_H
#include "edObjects.h"

#include <ed/map.h>

#include "RegStorage.h"

class EDOBJECTS_API RegMapStorage : public RegStorage
{
private:
	typedef ed::map<ObjectID, Registered *> RegList;
	RegList  store;

	bool locked;

	ObjectID stepID;
	ObjectID nextID;

protected:
	ObjectID allocID();

public:
	RegMapStorage(viFlag _types, ObjectID _minID, ObjectID _maxID, ObjectID stepID=1);
	~RegMapStorage();

	virtual ObjectID Insert(Registered *obj);
	virtual bool Remove(Registered *obj);

	virtual ObjectID ForceID(Registered *obj, ObjectID id);

	virtual Registered *Get(ObjectID id);

	virtual Registered *Next(ObjectID id, viFlag oftype);
	virtual Registered *Prev(ObjectID id, viFlag oftype);

	virtual int DeleteAll(viFlag mask);

	virtual int Count() const;

	virtual void serialize(Serializer &);
	virtual void serializeSelf(Serializer &);
	virtual void serializeObjects(Serializer &);

};

#endif // RegMapStorage_H
