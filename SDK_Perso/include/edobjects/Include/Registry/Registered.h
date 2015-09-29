#ifndef Registered_H
#define Registered_H
#include "edObjects.h"

#include "Serializer/Serializer.h"
#include "Serializer/Serializable.h"

// тип объекта с точки зрения внутреннего хранения и обработки
// различные типы описаны в viFlags.h
typedef unsigned int viFlag;
const viFlag viUnknown = 0;

// сквозной идентификатор
// 0 - не зарегистрирован
typedef unsigned int ObjectID;

class lLandObject;

// базовый класс для всех объектов
class EDOBJECTS_API Registered : public Common::FakeUnknown<Serializable>

{
private:
	friend class lLandObject;
	viFlag type;
	ObjectID id;

protected:
	// вызываются при смене типа
	virtual bool Register();
	virtual bool Unregister();

public:
	Registered(viFlag _type = viUnknown);
	virtual ~Registered();

	inline viFlag viType() const	{return type;}
	inline ObjectID ID() const	{return id;}
	int netHost() const;

    virtual void serialize(Serializer &);

	// форсировать установку нового ID (id должен быть в области, соответствующей типу)
	virtual bool ForceID(viFlag _type, ObjectID _id);

	virtual void viSetType(viFlag _type);
};

#endif // Registered_H