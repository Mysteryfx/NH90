#ifndef __USERINTERFACE_CURSOR_OWNER_H__
#define __USERINTERFACE_CURSOR_OWNER_H__

#include "ExpImpSpec.h"
#include <ed/string.h>

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR CursorOwner
{
public:
	CursorOwner(int cursor) : cursor_(cursor)
	{
	}

	void setCursor(int cursor) { cursor_ = cursor; } 
	int getCursor() { return cursor_; }

	void setName(const ed::string& name) { name_ = name; }
	const ed::string& getName() const { return name_; }

protected:
	int cursor_;
	ed::string name_;
};

#endif // __USERINTERFACE_CURSOR_OWNER_H__