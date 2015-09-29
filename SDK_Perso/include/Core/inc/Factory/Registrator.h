#ifndef __Registrator_h__
#define __Registrator_h__

#include "FactoryManager.h"

/*
	1.
	create access function in your DLL:
	void DLL_API getYourRegistry(FactoryManager **);
	realize it like:
	void DLL_API getYourRegistry(FactoryManager **fm)
	{
		(*fm) = 0;
		static CComPtr<FactoryManager> yourRegistry	= new FactoryManager;
		yourRegistry.CopyTo(fm);
	}
	2. 
	create your class:
	class MyClass {};
	in your MyClass.cpp file register factory on the stack:
	REGISTER_FACTORY(getYourRegistry, new YourFactory<MyClass>(CLSID_IMyClass));
*/
namespace Common {

class Registrator
{
public:
	typedef void Accessor(FactoryManager **);
    Registrator(Accessor accessor, Factory *factory) : 
        accessor_(accessor),
        factory_(factory)
	{
		SharedPtr<FactoryManager> factoryManager;
		accessor((FactoryManager **)&factoryManager);
		factoryManager->addFactory(factory);
	}

    ~Registrator()
    {
		SharedPtr<FactoryManager> factoryManager;
		accessor_((FactoryManager **)&factoryManager);
		factoryManager->removeFactory(factory_);
    }

private:
    Accessor* accessor_;
    Factory* factory_;
};

template <class T> struct RegistryActivator
{
	RegistryActivator(Registrator::Accessor accessor, T *&t)
	{
		SharedPtr<FactoryManager>	factoryManager;
		accessor((FactoryManager **)&factoryManager);
		t = dynamic_cast<T *>(factoryManager.get());
	}
	static void getFactoryManager(FactoryManager **factoryManager)
	{
		(*factoryManager) = 0;
		static SharedPtr<T> manager = new T;
		SharedPtr<FactoryManager> fm = manager;
		//fm.CopyTo(factoryManager);
		*factoryManager = fm;
		if (*factoryManager)
			(*factoryManager)->AddRef();
	}
};

} // namespace Common

#define REGISTER_FACTORY(accessor, factory) \
	static Common::Registrator registrator(accessor, factory);

#define REGISTER_FACTORY_EX(accessor, factory, name) \
	static Common::Registrator name##registrator(accessor, factory);

#define IMPLEMENT_REGISTRATOR(Manager, accessor, global) \
	static Common::RegistryActivator<Manager> global##RegistryActivator(accessor, global); \
	void accessor(Common::FactoryManager **factoryManager) \
	{ Common::RegistryActivator<Manager>::getFactoryManager(factoryManager); }

#endif
