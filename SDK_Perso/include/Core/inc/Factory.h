#ifndef CORE_FACTORY_H
#define CORE_FACTORY_H

#include "_config.h"
#include "ed/unordered_map.h"

#include <typeinfo>
#include <ed/string.h>

#ifndef ED_CORE_EXPORT
#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif // ED_CORE_EXPORT

namespace ed
{

/// Factory - object for creating other objects by their class name
class Factory
{
public:
	virtual ~Factory() {}

	/// get class count in this factory
	virtual int getCount() = 0;

	/// get class identifier (user class name)
	virtual const char* getIdentifier(int index) = 0;

	/// get class system name, as returned by typeid().name()
	virtual const char* getSystemId(int index) = 0;

	/// get user class name for this system id
	virtual const char* identify(const char* sysId) = 0;

	/// create object by it's class identifier (user class name)
	virtual void* create(const char* id) = 0;

	/// create object by it's class identifier (user class name)
	/// in defined placement
	virtual void* create(const char* id,void* p) = 0;

	/// Returns size of class object.
	virtual size_t getSize(const char* id)=0;
};

/// factory manager
class FactoryManager : public Factory
{
public:
	ED_CORE_EXPORT FactoryManager();
	ED_CORE_EXPORT virtual ~FactoryManager();

	/// add/remove factory
	virtual void add(Factory* factory);
	virtual void remove(Factory* factory);

	/// Factory interface
	virtual int getCount();	// not implemented
	virtual const char* getIdentifier(int index); // not implemented
	virtual const char* getSystemId(int index); // not implemented
	virtual const char* identify(const char* sysId);
	virtual void* create(const char* id);
	virtual void* create(const char* id,void* p);
	virtual size_t getSize(const char* id);

	ED_CORE_EXPORT static FactoryManager& instance();

protected:
	ed::unordered_map<ed::string, ed::string> names;	// systemId => userId
	ed::unordered_map<ed::string, Factory*> factories;	// userId => factory
};

// factory for a single class, registers itself in the global factory
// regClass<MyClass> myReg("MyClass");
template <class T>
class regClass : public Factory
{
public:
	regClass(const char* name)
	{
		id = name;
		FactoryManager::instance().add(this);
	}
	virtual ~regClass()
	{
		FactoryManager::instance().remove(this);
	}

	virtual int getCount()
	{
		return 1;
	}
	virtual const char* getIdentifier(int index)
	{
		return id;
	}
	virtual const char*	getSystemId(int index)
	{
		return typeid(T).name();
	}
	virtual const char* identify(const char* sysId)
	{
		return id;
	}
	virtual void* create(const char* id)
	{
		return new T;
	}
	virtual void* create(const char* id,void* p)
	{
		return new(p) T;
	}
	virtual size_t getSize(const char* id)
	{
		return sizeof(T);
	}

private:
	const char* id;
};

}

#endif // CORE_FACTORY_H
