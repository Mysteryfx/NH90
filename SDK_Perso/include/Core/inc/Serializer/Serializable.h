#ifndef __Serializable_h__
#define __Serializable_h__

#include "Factory/Identifiable.h"

class Serializer;

class Serializable : public Common::Identifiable
{
public:
    Serializable() {}
    virtual void serialize(Serializer &) {}
};

inline
Serializer& operator << (Serializer& str, Serializable& obj)
{
	obj.serialize(str);
	return str;
}

typedef Common::SharedPtr<Serializable> ISerializablePtr;


/* fix for strange linking problem: some classes make (Fake)Unknown<Serializable> members exported */
namespace Common {

template<>
class ED_CORE_EXTERN Unknown<Serializable> : public Serializable {
	void FinalRelease();
protected:
	Unknown();
	virtual ~Unknown();
};

template<>
class ED_CORE_EXTERN FakeUnknown<Serializable> : public Serializable {
	void FinalRelease();
protected:
	FakeUnknown();
	virtual ~FakeUnknown();
};

} // namespace Common

#endif

