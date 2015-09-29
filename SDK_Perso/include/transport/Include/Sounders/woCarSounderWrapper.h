#pragma once

#include "SoundTools.h"

#include "ed/string.h"
#include "Lua/Config.h"

class woCar;

class woCarSounderWrapper
{
public:
	woCarSounderWrapper(woCar* hostIn, Lua::Config& cfg);
	~woCarSounderWrapper();

public:
	bool updateSound();
	bool createSound();

	void setRadarRotationFlag(bool rotationFlag);
	bool getRadarRotationFlag();

private:
	woCar* host;

	ed::SounderParam param_modelTime, param_EngineRPM, param_TurbineRPM, 
						param_ThrottleState, param_BrakeState, param_IsHumanPlay,
						param_GearNum, param_Velocity, param_SurfType, param_StartRadarRotationSound;

	ed::SounderParam param_toPlay;

	class EngineParams
	{
	public:
		ed::SounderParam engineRPM, turbineRPM, 
							throttleState, brakeState, isHumanPlay,
							gearNum;
	};

	EngineParams engineParams;

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

// �����
	float deltaTime;
	float lastUpdateTime;
// �����

// ���� ������������� ����������� ���� �������� ������

	bool rotateFlag;

// ���� ������������� ����������� ���� �������� ������
};