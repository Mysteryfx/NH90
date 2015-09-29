#ifndef ccCockpitFactory_H
#define ccCockpitFactory_H

#include "Common.h"
#include "Factory/StandardFactory.h"

struct lua_State;

namespace cockpit
{

class Element;

class ceFactory
{
public:
	Element* create(const ed::string& classname);
	void regInScript(lua_State* L);
};

extern ceFactory cefactory;

template <class T>
struct elemCreator : public Common::FakeUnknown< Common::StandardFactory<T> >
{
	elemCreator(const ed::string& name)
	{
		addIdentifier(Common::Identifier(ed::string("cockpit::") + name));
		Common::globalFactoryManager->addFactory(this);
	}

	~elemCreator()
	{
		Common::globalFactoryManager->removeFactory(this);
	}
};

}
#define REGISTRY_ELEMENT_FACTORY(class)				elemCreator<class> reg##class (""#class"")
#define REGISTRY_ELEMENT_FACTORY_ALIAS(class,alias) elemCreator<class> reg##alias (""#alias"")

#endif // ccCockpitFactory_H
