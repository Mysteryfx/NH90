#pragma once

#include "WorldSound.h"
#include "osg/Vec2d"
#include "wTime.h"
#include "MathParser.h"

class woCar;

namespace Lua
{
	class Config;
}

//Звук движения машинок
class CarsSound
{
public:
	CarsSound();
	~CarsSound();
	
    void load(Lua::Config& cfg);
	void init(woCar* host);
	void update();
    void uninit();
    void startRadarSound();
    void stopRadarSound();

	typedef ed::vector<osg::Vec2d> FuncTable;

private:
	bool isInit() const;
	void updateEngineSound(double v, wModelTime curTime);
	void updateMoveSound(double v);
	void updateTurbineSound(double v, wModelTime deltaTime);
	void updateMoveNoise(double v, wModelTime deltaTime);
	void updateAccSound(wModelTime deltaTime);
	void updateSurfaceTypeMove(double v, wModelTime deltaTime);

	float calcBotEngineAndTransmissionParams(float v); // Расчет и имиитация двигателя и коробки передач. Функция возвращает расчитанный коофициент K1.

    // sound file names
    ed::string fnameEngine;
    ed::string fnameStarter;
    ed::string fnameMove;
	ed::string fnameStartMove;
	ed::string fnameEndMove;
    ed::string fnameTurn;
    ed::string fnameRadar;

	ed::string fnameEngineIdle;
	ed::string fnameEngineMax;
	ed::string fnameEngineAccStart;
	ed::string fnameEngineAccEnd;
	ed::string fnameMoveSound;
	ed::string fnameTurbineLoop;

	ed::string fnameMoveSoundNoise;

	ed::string engineIdleGainFormula;
	ed::string engineIdlePitchFormula;

	ed::string engineMaxGainFormula;
	ed::string engineMaxPitchFormula;

	ed::string turbineLoopGainFormula;
	ed::string turbineLoopPitchFormula;

	struct 
	{
        // sound sources
		Sound::Source engine;
		Sound::Source starter;
		Sound::Source move;
		Sound::Source turn;
        Sound::Source radar;

		FuncTable sndMovePitch;
		FuncTable sndMoveGain;
		FuncTable sndEnginePitch;
		FuncTable sndEngineVolV;
		FuncTable sndEngineVolA;
	} _snd;

	struct
	{
		Sound::Source engineIdle;
		Sound::Source engineMax;
		Sound::Source engineMaxAccStart;
		Sound::Source engineMaxAccEnd;
		Sound::Source moveSound;
		Sound::Source startMove;
		Sound::Source endMove;
		Sound::Source turbineLoop;
		Sound::Source noiseSound;

		Sound::Source waterMoveSound;

		FuncTable soundMovePitch;
		FuncTable soundMoveGain;

		FuncTable waterMovePitch;
		FuncTable waterMoveGain;
	} soundSource;

	osg::Vec2d _prevV, _smoothedAcc;
	wModelTime _lastUpdate;

	double _fwdAcc;
	bool _engineStop;
	wModelTime _timeStop;
	woCar* _host;

	bool useNewSound;

	MathParser* engineIdleGainParser;
	MathParser* engineIdlePitchParser;
	MathParser* engineMaxGainParser;
	MathParser* engineMaxPitchParser;
	MathParser* turbineLoopGainParser;
	MathParser* turbineLoopPitchParser;

	double dVg;
	double maxSpeed;
	int gearCount;
	int botCurrentGear;

	bool needPlayStartMoveSound;
	bool needPlayEndMoveSound;

	float timeToNextNoise;
	bool noiseFlag;

	wModelTime deltaTime;
	wModelTime engineMaxDeltaTime; // Используется только при отпускании педали газа.
	wModelTime engineMaxNeutralDeltaTime; // Дельта по времени для звука Max двигателя.
	wModelTime turbineNeutralDeltaTime; // Дельта по времени для звука турбины.
	wModelTime turbineOnceDeltaTime; // Дельта по времени для звука турбины для отпускания кнопки газа, проигрывается один раз.

	bool humanTookControl;
	bool needPlayOnceEngineMax;
	bool needPlayTurbineOnce;
	bool needPlayAccStartOnce;
	bool needPlayAccEndOnce;

	// Звуковые параметры сэмплов двигателя
	float engineMaxPitch;
	float engineMaxGain;
	// Звуковые параметры сэмплов двигателя

	// Звуковые параметры сэмплов турбины
	float turbinePitch;
	float turbineGain;
	// Звуковые параметры сэмплов турбины
	float engineNeutralRPM;

	// Продвинутая иммитация звука переключения передач
	ed::vector<float> gearRatios; // Таблица передаточных чисел.

	float engineMaxRPM; // Максимальные обороты двигателя.
	float engineMinRPM; // Минимальные обороты двигателя.
	float wheelRadius; // Радиус колеса.
	float mainGearRatio; // Передаточное число главной передачи.
	float unitMass; // Масса юнита.
	float K1; // Задает соотношение оборотов двигателя к максимальным.

	int currentGear; // Текущая передача.
	// Продвинутая иммитация звука переключения передач

	ed::string waterMoveSound;

	float oldRPM;
	float newRPM;
	float deltaRPM;
};