#ifndef __WeaponFlagEnum__
#define __WeaponFlagEnum__

#include "WorldGeneral.h"

typedef unsigned int WeaponFlags;

#undef WEAPON_FLAG_BEGIN
#define WEAPON_FLAG_BEGIN enum WeaponFlag : unsigned int {
#undef WEAPON_FLAG
#define WEAPON_FLAG(name, value) name = value,
#undef WEAPON_FLAG_END
#define WEAPON_FLAG_END };

#include "WeaponFlag.h"

#include "wsType.h"

#undef WEAPON_FLAG_END
#undef WEAPON_FLAG
#undef WEAPON_FLAG_BEGIN

WeaponFlags WORLDGENERAL_API getWeaponType(const wsType & ws_type);

#endif __WeaponFlagEnum__