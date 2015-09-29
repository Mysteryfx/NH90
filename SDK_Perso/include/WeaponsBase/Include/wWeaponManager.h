//                    file wWeaponManager.h
#pragma once

#include "IWeaponManager.h"
#include "IwInfo.h"
#include "wShapeTable.h"

#define TIME_FLIGHT_MAX 100000.0
 
class wWeaponManager : public IWeaponManager
{
public:

	struct InitNetShell
	{
		// netCreate
		ObjectID		idShell;
		cVector			pos;
		unsigned char	type;
		float			velocityOut;
		// updateStatus
		float			timeForm;
		cVector			vel;

		operator InitWeapon() const
		{
			InitWeapon initWeapon;
			initWeapon.pos.p = pos;
			initWeapon.type = wsType(wsType_Weapon, wsType_Shell, wsType_Shell_A, type);
			initWeapon.velocityOut = velocityOut;
			initWeapon.lifeTime = 1000.0f;
			initWeapon.idShell = idShell;
			return initWeapon;
		}
	};

	struct InitNetRocket : public InitNetShell
	{
		// updateStatus
		float			timeBegin;

		operator InitWeapon() const
		{
			InitWeapon initWeapon;
			initWeapon.pos.p = pos;
			initWeapon.type = globalInfo->Get_wsType(wShapeN_MissileTable, type);
			initWeapon.velocityOut = velocityOut;
			initWeapon.lifeTime = 1000.0f;
			initWeapon.idShell = idShell;
			return initWeapon;
		}
	};

	virtual IwWeapon*	create(WeaponType weaponType, InitWeapon& initWeapon);
	virtual bool		aiming(AimingWeapon& data);
};