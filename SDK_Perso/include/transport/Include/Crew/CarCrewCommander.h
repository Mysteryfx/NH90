#pragma once

#include "CarCrew.h"

#define IID_ICarCrewCommander Common::identify<CarCrewCommander>()

class CarCrewCommander : public CarCrew
{
public:
	CarCrewCommander();
	virtual ~CarCrewCommander();

public:
	virtual void createCrewMember(ObjectID hId, ed::string &cL, Lua::Config& c);
	virtual void initMessages();

	virtual CarCrew::CrewMemberType getMemberType();

private:

	static enum MessageType
	{
		NEXT_ROUND, // �������� ����������� �����
		CHANGE_ROUND // �������� ������� ��������� � ������
	};

	static enum MessageSubType
	{
		ATGM, // ����
		HEAT, // ���
		SABOT // ���
	};

	ed::vector<CrewMessage> cMessages;

protected:
	// ����

	virtual int getMessage(CrewMessageMap msg);

	// ����

public:
	virtual void stubMethod();
};