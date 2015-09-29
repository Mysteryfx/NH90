#ifndef __IGUN_CARRIAGE_H__
#define __IGUN_CARRIAGE_H__

#include "Math/Vector.h"
#include "Math/Position.h"
#include "Registry/Registered.h"

enum wcCoalitionName;

// Лафет
// Что-бы пушка могла запрашивать свое положение
class wIGunCarriage
{
public:
	//	положение и скорость ствола в глобальной системе координат и в текущее время
	virtual Math::Vec3d getShellFirePosition() = 0;	 //shell born position 
	virtual Math::Vec3d getShellFireDirection() = 0;
	virtual Math::Vec3d getDirection() = 0;
	virtual Math::Vec3d getVelocity() = 0;

	virtual ObjectID getID() const = 0;
	virtual wcCoalitionName getCoalition() const = 0;
	virtual unsigned char getState() const = 0;

	// Отдача
	// Импульс - снаряда
	virtual void acceptImpulse(double impulse, const Math::Vec3d & rot) = 0;

	// Некоторые эффекты приходится реализовывать на стороне владельца (например, огонь или взрывы)
	virtual void onGunStartBurst() = 0;		// начало очереди
	virtual void onGunStopBurst() = 0;		// конец очереди
	virtual void onGunShot() = 0;			// выстрел	
};


#endif 
