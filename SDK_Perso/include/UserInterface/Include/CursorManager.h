#ifndef __USERINTERFACE_CURSOR_MANAGER_H__
#define __USERINTERFACE_CURSOR_MANAGER_H__

#include "ExpImpSpec.h"
#include "ed/vector.h"
#include <ed/string.h>

class CursorOwner;
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR CursorManager
{
public:
	static void create();
	static void destroy();
	static CursorManager* get() { return instance_; }

	void addCursorOwner(CursorOwner* cursorOwner);
	void removeCursorOwner(CursorOwner* cursorOwner);
	bool isCursorVisible() const { return cursorVisible_; }

	void setSoftwareMode(bool mode);
	bool getSoftwareMode() { return softwareMode_; }

private:
	typedef ed::vector<CursorOwner*> CursorOwnerStack;

	CursorManager();

	void setCursor_(CursorOwner* cursorOwner);

	static CursorManager* instance_;
	CursorOwnerStack cursorOwnerStack_;
	bool cursorVisible_;
	bool softwareMode_;
};

#endif // __USERINTERFACE_CURSOR_MANAGER_H__