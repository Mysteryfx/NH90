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

// Флаг необходимости проигрывать звук вращения радара

	bool rotateFlag;

// Флаг необходимости проигрывать звук вращения радара
};