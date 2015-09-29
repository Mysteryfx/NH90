#ifndef __CONSTRAINT_BASE_H__
#define __CONSTRAINT_BASE_H__

#include "WeaponBlocks.h"

#include "Math/Vector.h"
#include "Math/Rotation3.h"

enum eConstraintType
{
	CONSTRAINT_FIXED,
	CONSTRAINT_RAIL,
	CONSTRAINT_ROTATION
};

class wAmmunition;

// Для запроса положения и скорости связи, а также для различных операций типа разъединения
class wIConstraintHost
{
public:
/*	// FIXME: wAmmunition тут уместно?
	virtual void attachClient(const wAmmunition* client) = 0;
	virtual void detachClient(const wAmmunition* client) = 0;*/


	// Это все относится к хосту
	virtual Math::Vec3d getGlobalPosition() = 0;
	virtual Math::Vec3d getGlobalVelocity() = 0;
	virtual Math::Matrix3d getGlobalOrientation() = 0;		
	
	// В локальной системе координат (хоста)
	virtual Math::Vec3d getLocalAcceleration() = 0;
	virtual Math::Vec3d getLocalAngularVelocity() = 0;
	virtual Math::Vec3d getLocalAngularAcceleration() = 0;

	virtual void acceptImpulse(const Math::Vec3d& pos, const Math::Vec3d& impulse) = 0;
};


// Сочленение считается жестко закрепленным на хосте со смещением position_ (в системе координат хоста) и ориентацией
// orientation_
class WEAPON_BLOCKS_API wConstraintBase
{
public:
	wConstraintBase();
	wConstraintBase(wIConstraintHost* host, const Math::Vec3d& position, const Math::Matrix3d& orientation);
	virtual ~wConstraintBase() {};

	virtual eConstraintType getType() const = 0;

	// В мировой системе координат
	Math::Vec3d getGlobalPosition();
	Math::Vec3d getGlobalVelocity();
	Math::Matrix3d getGlobalOrientation();
	Math::Vec3d getGlobalAngularVelocity();

	Math::Vec3d getLocalAngularVelocity();

	// Позиция и ориентация в системе координат хоста
	Math::Vec3d getHostLocalPosition() { return position_; }
	Math::Matrix3d getHostLocalOrientation() { return orientation_; }

	wIConstraintHost* getHost() { return host_; }

protected:
	wIConstraintHost* host_;
	Math::Vec3d position_;
	Math::Matrix3d orientation_;
};


#endif 
