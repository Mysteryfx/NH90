#pragma once

#include "CarCrew.h"

#include "woCar.h"

#define IID_ICarCrewGunner Common::identify<CarCrewGunner>()

class CarCrewGunner : public CarCrew
{
public:
	CarCrewGunner();
	virtual ~CarCrewGunner();

public:
	virtual void createCrewMember(ObjectID hId, ed::string &cL, Lua::Config& c);
	virtual void initMessages();

	virtual CarCrew::CrewMemberType getMemberType();

private:

	static enum MessageType
	{
		ENEMY_HIT, // Попадание
		ENEMY_BURN, // Враг горит
		ENEMY_DESTROYED, // Враг уничтожен
		GETTING_DAMAGE // В нас попали
	};

	static enum MessageSubType
	{
		NO_PENETRATION, // Не пробили броню
		YES_PENETRATION, // Пробили броню
		COMMON // Учитывается только MesageType
	};

	ed::vector<CrewMessage> cMessages;

protected:
	virtual int getMessage(CrewMessageMap msg);

public:
	
	// События

	virtual void enemyHit();
	virtual void enemyBurn();
	virtual void enemyHitWArmorPenet();
	virtual void enemyDestroyed();
	virtual void gettingDamage();

	// События
};