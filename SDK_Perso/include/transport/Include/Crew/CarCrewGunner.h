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
		ENEMY_HIT, // ���������
		ENEMY_BURN, // ���� �����
		ENEMY_DESTROYED, // ���� ���������
		GETTING_DAMAGE // � ��� ������
	};

	static enum MessageSubType
	{
		NO_PENETRATION, // �� ������� �����
		YES_PENETRATION, // ������� �����
		COMMON // ����������� ������ MesageType
	};

	ed::vector<CrewMessage> cMessages;

protected:
	virtual int getMessage(CrewMessageMap msg);

public:
	
	// �������

	virtual void enemyHit();
	virtual void enemyBurn();
	virtual void enemyHitWArmorPenet();
	virtual void enemyDestroyed();
	virtual void gettingDamage();

	// �������
};