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

		ed::string unitName; // ��� ����� �� �������.
		unsigned int seasonId; // ����� ����.
		unsigned int surfType; // ��� ������������ �����������.
		double atmoTemp; // ����������� ������� � ������ �����. ��������� � �������� �������.
		double geoPotH; // ���������������� ������.
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

// ���������� �� �����
	Graphics::ModelConnector* connectorL;
	Graphics::ModelConnector* connectorR;
// ���������� �� �����

// ����� ��� ���
	bool leftLeg;
	bool rightLeg;
// ����� ��� ���

private:
	void isFootOnTheGround(); // ������ ��������� �� �� ����� ����� � ��������
};