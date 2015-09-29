#ifndef __DESCRIPTOR_BASE_H__
#define __DESCRIPTOR_BASE_H__

#include <ed/string.h>

class wDatabaseLoader;

#define REGISTER_DESCRIPTOR(x)	\
	wDescriptorBase* __create##x(const ed::string& name) \
	{ return new x(name); } \
	const bool __registered##x = (wDescriptorFactory::instance()->registerDescriptorType(#x, &__create##x), true);

#define DESCRIPTOR_STANDARD_CONSTRUCTOR(classname, base) classname(const ed::string& name) : base(name) {}

class wDatabaseLoader;

class wDescriptorBase
{
public:
	wDescriptorBase(const ed::string& name) : name_(name) {};
	virtual ~wDescriptorBase() {};

	virtual void load(wDatabaseLoader* loader) = 0;

	const ed::string& getName() const { return name_; }

protected:
	ed::string name_;

	friend class wDatabaseLoader;
};

#endif
