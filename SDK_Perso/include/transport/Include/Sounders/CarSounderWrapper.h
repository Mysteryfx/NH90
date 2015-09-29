#pragma once

#include "ed/string.h"
#include "Lua/Config.h"

class woCar;

class CarSounderWrapper
{
public:
	CarSounderWrapper(woCar* hostIn, Lua::Config& cfg);
	~CarSounderWrapper();

public:
	bool updateSound();
	bool createSound();

	void setRadarRotationFlag(bool rotationFlag);
	bool getRadarRotationFlag();

private:
	woCar* host;
	
	ed::string selectSounder();

	void getCarChassisParams(Lua::Config& cfg);

// ����������� ��������� ����� ������������ �������

	float calcBotEngineAndTransmissionParams(float v);

	ed::vector<float> gearRatios; // ������� ������������ �����.

	float engineMaxRPM; // ������������ ������� ���������.
	float engineMinRPM; // ����������� ������� ���������.
	float wheelRadius; // ������ ������.
	float mainGearRatio; // ������������ ����� ������� ��������.
	float unitMass; // ����� �����.
	float K1; // ������ ����������� �������� ��������� � ������������.

	int currentGear; // ������� ��������.

// ����������� ��������� ����� ������������ �������

// ������ ��� ����������� �� ������� ����

	int exploreSurfType();

// ������ ��� ����������� �� ������� ����

// ���� ������������� ����������� ���� �������� ������

	bool rotateFlag;

// ���� ������������� ����������� ���� �������� ������
};