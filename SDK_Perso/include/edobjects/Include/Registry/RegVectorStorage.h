#ifndef RegVectorStorage_H
#define RegVectorStorage_H

#include <ed/vector.h>

#include "RegStorage.h"

class EDOBJECTS_API RegVectorStorage : public RegStorage
{
protected:
	typedef ed::vector<Registered *> RegList;
	RegList  store;

	int count;
	ObjectID lastID;

public:
	RegVectorStorage(viFlag _types, ObjectID _minID, ObjectID _maxID);
	~RegVectorStorage();

	virtual ObjectID Insert(Registered *obj);
	virtual bool Remove(Registered *obj);

	virtual ObjectID ForceID(Registered *obj, ObjectID id);

	virtual Registered *Get(ObjectID id);

	virtual Registered *Next(ObjectID id, viFlag oftype);
	virtual Registered *Prev(ObjectID id, viFlag oftype);

	virtual int DeleteAll(viFlag mask);

	virtual int Count() const	{return count;}
	
	virtual void serialize(Serializer &);
	virtual void serializeSelf(Serializer &);
	virtual void serializeObjects(Serializer &);

	virtual ObjectID GetLastID();
};

#endif // RegVectorStorage_H