#pragma once

#include "ed/string.h"

#include "Modeler/ModelConnector.h"

class woCar;

class InfantrySounderWrapper
{
public:
	InfantrySounderWrapper(woCar* hostIn);
	~InfantrySounderWrapper();

public:
	bool createSound();
	bool updateSound();

private:
	struct UnitParams
	{
		UnitParams(ed::string uName)
		{
			unitName = uName;
			seasonId = 1;
			surfType = 0;
			atmoTemp = 0.01f;
			geoPotH = 0.0f;
		}

		ed::string unitName; // Имя юнита из скрипта.
		unsigned int seasonId; // Время года.
		unsigned int surfType; // Тип подстилающей поверхности.
		double atmoTemp; // Температура воздуха в данной точке. Приведено к градусам Цельсия.
		double geoPotH; // Геопотенциальная высота.
	};

	enum SeasonId
	{
		SUMMER = 1,
		WINTER = 2,
		SPRING = 3,
		AUTUM = 4
	};

private:
	woCar* host;
	UnitParams* unitParams;

// Коннекторы на ногах
	Graphics::ModelConnector* connectorL;
	Graphics::ModelConnector* connectorR;
// Коннекторы на ногах

// Флаги для ног
	bool leftLeg;
	bool rightLeg;
// Флаги для ног

private:
	void isFootOnTheGround(); // узнаем коснулись ли мы земли ногой в движении
};