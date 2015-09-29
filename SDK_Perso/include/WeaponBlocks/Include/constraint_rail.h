#ifndef __CONSTRAINT_RAIL_H__
#define __CONSTRAINT_RAIL_H__

#include "WeaponBlocks.h"

#include "constraint_base.h"

class wConstraintRail;

class wIRailOwner		// Чтобы клиент мог отсоединяться
{
public:
	virtual void detachClientFromRail(wConstraintRail* rail) = 0;
};

class WEAPON_BLOCKS_API wConstraintRail : public wConstraintBase
{
public:
	wConstraintRail(wIConstraintHost* host, wIRailOwner* owner, const Math::Vec3d& position, const Math::Matrix3d& orientation, double length);
	virtual ~wConstraintRail();

	virtual eConstraintType getType() const { return CONSTRAINT_RAIL; }

	// Получить силы инерции для точки r, движущейся со скоростью
	// В системе координат сочленения
	Math::Vec3d getInertialAccelerations(const Math::Vec3d& r, const Math::Vec3d& v);

	// Длина рельсы
	double getLength() { return length_; }

	void detachClient();			// Отсоединить клиента

	void acceptImpulse(double impulse);

protected:
	wIRailOwner*	owner_;
	double			length_;

	Math::Matrix3d	inv_orientation_;		// TODO: Это надо будет когда-нибудь удалить, а умножать на транспонированную
};

#endif 
