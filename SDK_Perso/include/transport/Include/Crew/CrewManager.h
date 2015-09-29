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

// ������ ��� ������ �������
friend class CrewGunner;
// ������ ��� ������ �������

// �������������
private:
	void initCrew(Lua::Config& conf);
	bool initSounder();
// �������������

// ������� �������
public:
	void playerTakeControl();
	void playerReleaseControl();

// ������� ��� �������
	void enemyHit(float caliber, float life);
	void enemyNonFatalDamage(); // ���� ���� �� �������� ��������� ���� � ����� �� ������
	void enemyFatalDamage();
	void weGetDamage();
// ������� ��� �������

// ������� �������

// ������� ������� � �������� ��������
private:
	woCar* host;
	CrewSoundWrapper* crewSoundWrapper;
// ������� ������� � �������� ��������

// ��������� ������
private:
	CrewSoundWrapper* getSoundWrapper();
	CrewGunner* getGunner();
// ��������� ������

// ��������� ���������� � �������
private:
	ed::string crewLocale;

	CrewGunner* crewGunner;
// ��������� ���������� � �������
};