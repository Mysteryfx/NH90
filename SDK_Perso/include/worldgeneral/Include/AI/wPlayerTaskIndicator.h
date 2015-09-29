#ifndef __IwPlayerTask__
#define __IwPlayerTask__

//The class indicates to the player the task that is assigned to the player or to the player's group.
//Tasks can be nested into each other. For example parent task - ComboTask, nested task - AttackGroup.
//Task can only be finished by wPlayerTaskIndicator::resetTask() call.

#include "WorldGeneral.h"

namespace AI
{

class AttackGroup;
class AttackUnit;
class Bombing;
class AttackMapObject;
class BombingRunway;
class Task;
class Orbit;
class Land;
class Refueling;
class Follow;
class Escort;

class WORLDGENERAL_API wPlayerTaskIndicator
{
public:
	virtual ~wPlayerTaskIndicator();
	virtual void setTask(AttackGroup * attackGroup);
	virtual void setTask(AttackUnit * attackUnit);
	virtual void setTask(Bombing * bombing);
	virtual void setTask(AttackMapObject * attackMapObject);
	virtual void setTask(BombingRunway * bombingRunway);
	virtual void setTask(Orbit * orbit);
	virtual void setTask(Land * land);
	virtual void setTask(Refueling * refueling);
	virtual void setTask(Follow * follow);
	virtual void setTask(Escort * escort);
	virtual void setTask(Task * task);
	virtual void resetTask(Task * task);
};

}

#endif __IwPlayerTask__