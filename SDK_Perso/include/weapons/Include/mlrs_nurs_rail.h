#ifndef __MLRS_NURS_RAIL_H__
#define __MLRS_NURS_RAIL_H__

#include "constraint_rail.h"

class cLauncherRocket;
class cPosition;

class wAmmunition;

// Этот класс - detached (сам себя удаляет)
class wMLRSRail : public wIConstraintHost, public wIRailOwner
{
public:
	wMLRSRail(cLauncherRocket* owner, wAmmunition* rocket, const cPosition& pos, double len, double impulse_factor);
	~wMLRSRail();

    // After this method is called it is only possible to call detachClientFromRail.
    // All other calls will result in access violation.
    void ownerDestroyed();

	// from wIContraintHost
	virtual Math::Vec3d getGlobalPosition();
	virtual Math::Vec3d getGlobalVelocity();
	virtual Math::Matrix3d getGlobalOrientation();		
	// В локальной системе координат (хоста)
	virtual Math::Vec3d getLocalAcceleration();
	virtual Math::Vec3d getLocalAngularVelocity();
	virtual Math::Vec3d getLocalAngularAcceleration();

	// from wIRailOwner
	virtual void detachClientFromRail(wConstraintRail* rail);

	// Accept impulse
	virtual void acceptImpulse(const Math::Vec3d& pos, const Math::Vec3d& impulse);

protected:
	wConstraintRail rail_;
	wAmmunition* rocket_;

	cLauncherRocket* owner_;

	double impulseFactor_;
};

#endif