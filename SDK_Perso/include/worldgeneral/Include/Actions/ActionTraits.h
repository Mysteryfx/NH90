#ifndef __ACTION_TRAITS_H__
#define __ACTION_TRAITS_H__

#include "WorldGeneral.h"

#include "ActionEntry.h"

class WORLDGENERAL_API ActionTraits
{
private:
	ActionTraits();

public:
	static bool isVideoAction(Action action);
	static bool isAnalogVideoAction(Action action);
	static bool isDigitalVideoAction(Action action);

    static bool isCursorAction(Action action);

	static bool isAdminsAction(Action action);
	static bool isVideoEditAction(Action action);
};

#endif