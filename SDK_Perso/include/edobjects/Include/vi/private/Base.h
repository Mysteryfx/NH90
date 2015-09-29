#ifndef __VI_BASE_H
#define __VI_BASE_H

class viObjectManager_Implement;

namespace vi
{
class Manager;

struct StaticObject
{
	enum STORAGE
	{
		UNEQUALBUILDINGS=0, 
		DISTRICTBUILDINGS=1, 
	}storage;
	uint32_t id;
};

template <class T>
class Base
{
	friend class vi::Manager;
	friend class viObjectManager_Implement;

public:
	mutable union
	{
		T* instancedobject;
		StaticObject staticobject;
	}ref;

	enum REFTYPE
	{
		NULLOBJECT = 0, 
		INSTANCEDOBJECT = 1,
		STATICOBJECT = 2,
	};
	mutable REFTYPE reftype;
public:
	Base()
	{
		reftype = NULLOBJECT;
	}
	Base(T* instancedobject)
	{
		reftype = INSTANCEDOBJECT;
		ref.instancedobject = instancedobject;
	}
	bool isValid()
	{
		if(reftype == INSTANCEDOBJECT)
			return ref.instancedobject != 0;
		else if(reftype == STATICOBJECT)
			return ref.staticobject.id != 0;
		return false;
	}
	T* instance() const
	{
		if(reftype == INSTANCEDOBJECT)
			return ref.instancedobject;
		return doinstance();
	}
	T* doinstance() const
	{
		if(reftype != STATICOBJECT)
			return 0;
		assert(false && "not implemented");
		return 0;
	}
	template<class T2>
	void cast(const Base<T2>& arg)
	{
		reftype = (Base<T>::REFTYPE)arg.reftype;
		if(reftype == STATICOBJECT)
		{
			ref.staticobject = arg.ref.staticobject;
		}
		else if(reftype == INSTANCEDOBJECT)
		{
			ref.instancedobject = dynamic_cast<T*>(arg.ref.instancedobject);
		}
	}
};

}

#endif
