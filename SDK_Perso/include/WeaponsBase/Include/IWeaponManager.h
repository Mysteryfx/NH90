//                    file IWeaponManager.h
#pragma once

#include "WeaponsBase.h"
#include "wMovingObject.h"
class IwWeapon;

typedef enum WeaponType
{
	wtNucleus = 1,
	wtTailNucleus,
	wtUnguidedRocket,
	wtGrenade,
	wtSmokeGrenade
};

class WEAPONSBASE_API IWeaponManager
{
public:
	struct WEAPONSBASE_API InitWeapon
	{
		InitWeapon();		

		woPointer	pHost;

		cPosition	pos;
		cVector		vel;
		cVector		target;

		wsType		type;
		float		velocityOut;
		wModelTime	lifeTime;
		wModelTime	timeFlight;

		ObjectID	idShell;
		bool		netShell;
	};

	struct WEAPONSBASE_API AimingWeapon
	{
		AimingWeapon();

		//	Input
		cVector hostPos, hostVel;
		cVector targetPos, targetVel, targetAcc;
		float nucleusVel, tAcc;

		// для расчета точки прицеливания с учетом ветра
		bool	advanced_wind;
		float	velocityOut;
		
		//	Output
		wModelTime timeFlight;
		cVector point, dir;

		friend Serializer& operator << (Serializer& serializer, AimingWeapon& aimingw);
		AimingWeapon& operator = (const AimingWeapon&);
	};

	virtual ~IWeaponManager() 
	{}

	virtual IwWeapon*	create(WeaponType weaponType, InitWeapon& initWeapon) = 0;

	virtual bool		aiming(AimingWeapon& data) = 0;
};

extern  WEAPONSBASE_API IWeaponManager* globalWeaponManager; 
