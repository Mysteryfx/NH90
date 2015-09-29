#ifndef _Damage_
#define _Damage_

//Calculations of damage from weapon's warhead hit or/and explosion

#include "WeaponsBase.h"
#include "cLinear.h"
#include "armored_object_interface.h"

struct Damage
{
	bool rebound; //AP-shell rebound
	bool pierced; //Is armor is pierced
	double cumulative; //Damage to the object that was hit
	double effect; //Damage to all the objects around
    double decayFactor; // depends on (explosiveMass/fullMass 1.0 by default)
};

struct Warhead
{
	enum Type
	{
		TYPE_VOID = -1,
		TYPE_AP,	//Pierces armor and penetrates object, 
					//Makes damage to object by kinetic energy and by explosive (optional),
					//Makes damage to surrounding objects by explosive (optional)
		TYPE_SHAPED_EXPLOSIVE	//Shaped explosive warhead activates in front of object surface and penetrates it,
								//Makes damage to object by shaped explosive,
								//Makes damage to surrounding objects by explosive (optional)
	} type;
	//Common
	double mass; //Mass of the whole warhead, kg
	double explosiveMass; //Mass of explosive, kg
	//AP
	double caliber; //AP-caliber, m. Used in armor piercing calculations
	double piercingMass; //Mass of piercing cap, kg
	//Shaped explosive
	double shapedExplosiveMass; //Mass of explosive for shaped-explosive warhead, kg
	double armorMaxThickness; //Maximal thickness of an armor that can be penetrated by warhead, m
};

//Returns damage to the non-armored object
WEAPONSBASE_API Damage hitObject(const Warhead & warhead, double fullVelLength);

//Returns damage to the armored object
WEAPONSBASE_API Damage hitArmoredObject(const Warhead & warhead, double fullVelLength, double cosAngle, const wIArmoredTarget::ArmorData & armorData);

//Returns damage to the objects around due the explosion on the groun or in air
WEAPONSBASE_API Damage explode(const Warhead & warhead);

#endif
