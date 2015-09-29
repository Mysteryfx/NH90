#pragma once

#include "Serializer/Serializable.h"
#include "Registry/Registered.h"

#include "wMovingObject.h"

#include "Lua/Config.h"
#include <Lua/CheckedLoader.h>

#define IID_ICarCrew Common::identify<CarCrew>()

class CarCrew : public Common::FakeUnknown<Serializable>
{
public:
	CarCrew();
	virtual ~CarCrew();

public:
	static enum CrewMemberType
	{
		COMMANDER,
		GUNNER,
		INVALID
	};

	static enum CrewMessageMap
	{
		COMMANDER_NEXT_ATGM,
		GUNNER_HIT_COMMON,
		GUNNER_ENEMY_BURN,
		GUNNER_HIT_ARMOR_PENETRATION,
		GUNNER_ENEMY_DESTROYED,
		GUNNER_GETTING_DAMAGE
	};

	struct MessageFile
	{
		MessageFile()
		{
			fileName.clear();
			voiceId = 31337;
		}

		ed::string fileName;
		int voiceId;
	};

	struct CrewMessage
	{
		CrewMessage()
		{
			msgType = 0;
			msgSubType = 0;
			msgFiles.clear();
			msgTiming = 0.0f;
			msgDelay = 0.0f;
		}

		unsigned int msgType;
		unsigned int msgSubType;
		ed::vector<MessageFile> msgFiles;
		float msgTiming;
		float msgDelay;
	};

	virtual void createCrewMember(ObjectID hId, ed::string &cL, Lua::Config& c);
	virtual void initMessages();

	virtual CrewMemberType getMemberType();

protected:
	ObjectID hostId; // ID ������� ������ ������� ������� �� ��������
	woPointer carPointer; // ��������� �� ������� ������ ������� ������� �� ��������

protected:

	ed::string crewLocale; // ���� �� ������� ������� ������
	CrewMemberType memberType; // ��� ����� �������
	ed::string memberName; // ��� ����� �������
	float memberHp; // ���������� ������� �������� � ����� �������

	ed::string messageConfigFile; // ���� ����������� ���
	ed::string baseDir; // ������� �� �������
	ed::string searchSubDir; // ���������� ������ ������

	virtual int getMessage(CrewMessageMap msg);

public:

	// �������

	// �������
	virtual void enemyHit();
	virtual void enemyBurn();
	virtual void enemyHitWArmorPenet();
	virtual void enemyDestroyed();
	virtual void gettingDamage();
	// �������

	// �������
};