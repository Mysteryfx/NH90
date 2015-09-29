#pragma once

class CrewManager;

class CrewGunner
{
public:
	CrewGunner(CrewManager* myManager);
	~CrewGunner();

public:
	void enemyHit();
	void enemyBurn();
	void enemyHitWArmorPenet();
	void enemyDestroyed();
	void gettingDamage();

private:
	CrewManager* manager;
};