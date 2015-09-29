//                    file IWeapon.h
#pragma once

#include "WeaponsBase.h"
#include "wMovingObject.h"
#define WEAPON_DRAW_ARGS_MAX 30

class WEAPONSBASE_API IwMissile
{
public:
	virtual ~IwMissile(){}
	virtual void MissileSetTarget(const cVector& tpos) = 0;
    virtual wModelTime	getShotTime() = 0;
};

class WEAPONSBASE_API IwWeapon : public MovingObject, public IwMissile
{
public:
	virtual ~IwWeapon() 
	{}

	virtual woPointer	getHost() = 0;
	virtual void        MissileSetTarget(const cVector& tpos) {};
	
//	virtual wModelTime	getShotTime() = 0;
//	virtual float		getExpMass() = 0;
};

