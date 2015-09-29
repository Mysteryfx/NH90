#pragma once

#include "Lua/Config.h"

#include "Crew/CrewGunner.h"

#include "Crew/CrewSoundWrapper.h"

class woCar;

enum CrewMemberType
{
	COMMANDER,
	GUNNER,
	LOADER
};

class CrewManager
{
public:
	CrewManager(woCar* hostCar, Lua::Config& conf);
	~CrewManager();

// Доступ для членов экипажа
friend class CrewGunner;
// Доступ для членов экипажа

// Инициализация
private:
	void initCrew(Lua::Config& conf);
	bool initSounder();
// Инициализация

// Внешние события
public:
	void playerTakeControl();
	void playerReleaseControl();

// События для стрелка
	void enemyHit(float caliber, float life);
	void enemyNonFatalDamage(); // Если цель не получила фатальный урон и сразу не умерла
	void enemyFatalDamage();
	void weGetDamage();
// События для стрелка

// Внешние события

// Внешние объекты с которыми работаем
private:
	woCar* host;
	CrewSoundWrapper* crewSoundWrapper;
// Внешние объекты с которыми работаем

// Служебные методы
private:
	CrewSoundWrapper* getSoundWrapper();
	CrewGunner* getGunner();
// Служебные методы

// Остальные переменные и объекты
private:
	ed::string crewLocale;

	CrewGunner* crewGunner;
// Остальные переменные и объекты
};