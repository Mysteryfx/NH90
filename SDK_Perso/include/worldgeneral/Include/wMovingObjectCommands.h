#ifndef __wMovingObjectCommands_h__
#define __wMovingObjectCommands_h__

#include "WorldGeneral.h"

class MovingObject;

class WORLDGENERAL_API MovingObjectCommand
{
public:
	virtual ~MovingObjectCommand() {;}
	virtual void perform(MovingObject * obj) const;
};

class WORLDGENERAL_API SwitchEPLRS : public MovingObjectCommand
{
public:
	SwitchEPLRS(bool onOffIn, int groupIdIn) : onOff(onOffIn), groupId(groupIdIn) {;}
	virtual void perform(MovingObject * obj) const;
	bool	onOff;
	int		groupId;
};

#endif