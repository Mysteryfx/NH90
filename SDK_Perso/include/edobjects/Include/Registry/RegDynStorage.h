#ifndef RegDynStorage_H
#define RegDynStorage_H

#include "./RegMapStorage.h"


class EDOBJECTS_API RegDynStorage : public RegMapStorage {
public:
	// для работы сети - на каждой машине выставляется свой hostID
	// все ID выдаваемые стандартным путем получают hostID в нижних 8 битах
	static unsigned char hostID;

	RegDynStorage(viFlag _types, ObjectID _minID, ObjectID _maxID);
	~RegDynStorage();

	virtual ObjectID Insert(Registered *obj);

	// Для работы сети необходима следующая функциональность:
	// нижние 8 бит id - маска хоста
	// если верхние не 0 - стандартная процедура
	// иначе - выставить верхние 24 бита
	virtual ObjectID ForceID(Registered *obj, ObjectID id);

private:
	static ObjectID roundUp(ObjectID);

}; // class RegDynStorage


#endif /* RegDynStorage_H */
