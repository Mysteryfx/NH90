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
		NEXT_ROUND, // Следущим боеприпасом будет
		CHANGE_ROUND // Поменять текущий боеприпас в стволе
	};

	static enum MessageSubType
	{
		ATGM, // ПТУР
		HEAT, // ОФС
		SABOT // БПС
	};

	ed::vector<CrewMessage> cMessages;

protected:
	// Звук

	virtual int getMessage(CrewMessageMap msg);

	// Звук

public:
	virtual void stubMethod();
};