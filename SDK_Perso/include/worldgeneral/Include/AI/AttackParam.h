//Attack parameters for attack tasks

#ifndef _AttackParam_h_
#define _AttackParam_h_

#include "WorldGeneral.h"
#include "WeaponFlagEnum.h"

namespace Lua
{
	class Config;
}

namespace AI
{

struct AttackParam
{
	enum
	{
		NO_PARAMETERS		= 0,
		ATTACK_DIRECTION	= 1 << 1, //Attack direction is defined
		ATTACK_ALTITUDE		= 1 << 2, //Attack altitude (above target) is defined
		MIN_DISTANCE		= 1 << 3, //Minimal distance from the target is defined, N/A
		CARPET_ATTACK		= 1 << 4, //Carpet bombing is required, N/A
		ATTACK_QTY_LIMIT	= 1 << 5, //Attack quantity limit is defined
		GROUP_ATTACK		= 1 << 6  //Group attack is required
	};
	enum ExpendMode
	{
		EXPEND_AUTO, EXPEND_QTY, EXPEND_RATIO
	};
	enum AttackType
	{
		ATTACK_TYPE_AUTO, ATTACK_TYPE_DIVE, ATTACK_TYPE_LEVEL
	};
	AttackParam() { clear();}
	inline void		clear()
	{
		flags = 0;
		weaponType = AllWeapon;
		expendMode = EXPEND_AUTO;
		attackQty = 0;
		attackType = ATTACK_TYPE_AUTO;
	}
	void WORLDGENERAL_API load(Lua::Config & config);
	unsigned int	flags;
	typedef unsigned int WeaponType;
	WeaponType		weaponType; //Weapon types allowed to use
	ExpendMode		expendMode;
	unsigned int	expendQty; //Has effect if expendMode = EXPEND_QTY
	float			expendRatio; //Has effect if expendMode = EXPEND_RATIO
	unsigned int	attackQty; //Maximal quantity of attack
	float			attackAltitude; //Attack altitude relative to the target altitude
	float			attackDirection; //Attack direction from the target
	float			minDistance; //Minimal distance to the target, N/A
	AttackType		attackType; //Attack type required: dive, level
	float			carpetLength; //Length of carpet bombing, N/A
};

//Intersects attack parameters
AttackParam operator & (const AttackParam & left, const AttackParam & right);

//Combines attack parameters
AttackParam operator | (const AttackParam & left, const AttackParam & right);

}

#endif _AttackParam_h_