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

// Продвинутая иммитация звука переключения передач

	float calcBotEngineAndTransmissionParams(float v);

	ed::vector<float> gearRatios; // Таблица передаточных чисел.

	float engineMaxRPM; // Максимальные обороты двигателя.
	float engineMinRPM; // Минимальные обороты двигателя.
	float wheelRadius; // Радиус колеса.
	float mainGearRatio; // Передаточное число главной передачи.
	float unitMass; // Масса юнита.
	float K1; // Задает соотношение оборотов двигателя к максимальным.

	int currentGear; // Текущая передача.

// Продвинутая иммитация звука переключения передач

// Узнаем тип поверхности по которой едем

	int exploreSurfType();

// Узнаем тип поверхности по которой едем

// Время
	float deltaTime;
	float lastUpdateTime;
// Время

// Флаг необходимости проигрывать звук вращения радара

	bool rotateFlag;

// Флаг необходимости проигрывать звук вращения радара
};