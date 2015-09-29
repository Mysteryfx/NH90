#ifndef __Identifier_h__
#define __Identifier_h__

#include <ed/string.h>
#include <typeinfo>
#include "_config.h"

namespace Common {

/**
 * The Identifier class represents methods for operating identification mechanism.
 **/

class Identifier
{
public:
	ed::string _class_name;

	Identifier() {}
	Identifier(const ed::string& className) : _class_name(className) {}
	Identifier(const char* className) : _class_name(className) {}
	~Identifier() {}

	void                setClassName(const char *className) { _class_name = className; }
	const ed::string&  getClassName() const { return _class_name; }

	bool                operator==(const Identifier &id) const { return _class_name == id._class_name; }
	bool                operator!=(const Identifier &id) const { return _class_name != id._class_name; }
	bool                operator<(const Identifier &id) const { return _class_name < id._class_name; }
};

template <class T> inline
Identifier identify(T *t = 0)
{
	return Identifier(typeid(T).name());
}

} // namespace Common

using Common::Identifier;

#endif
