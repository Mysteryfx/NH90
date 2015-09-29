#pragma once

#include "Lua/Config.h"
#include "wTime.h"
#include "Renderer/DynamicParams.h"

class woCar;

namespace InfantryAnimation
{
	enum AnimState
	{
		ANIM_STOP = 0,
		WALK_ACC = 1,
		WALK_CYCLE = 2,
		WALK_TO_STOP = 3,
		WALK_TO_RUN = 4,
		RUN_ACC = 5,
		RUN_CYCLE = 6,
		RUN_TO_STOP = 7,
		RUN_TO_WALK = 8,
		ANIM_STATE_ERROR = 9
	};

	enum SpeedState
	{
		SPD_STOP = 0,
		SPD_WALK = 1,
		SPD_RUN = 2,
		SPD_ERROR = 3
	};
}

class InfantryAnimator
{
public:
	InfantryAnimator(woCar* h, Lua::Config& c);
	~InfantryAnimator();

	friend class HumanPlayable;

public:
	void update(wModelTime deltaTime, float velocity);
	bool getAnimArgVal(int argId, float& argVal); // Возвращает значение аргумента анимации

	bool playDeadAnimation(wModelTime deltaTime);

private:
	bool readConfig(Lua::Config& c);
	bool getSpeedAtWayPoint(int wayPointId, float& speed);

	void selectSpeedState(float velocity);
	void aiAnimator(wModelTime deltaTime, float velocity, InfantryAnimation::AnimState animState);
	void humanAnimator(wModelTime deltaTime);

private:
	woCar* host;
	Graphics::DParamList drawParams;

	InfantryAnimation::SpeedState speedState;
	InfantryAnimation::AnimState animState;

	bool isAnimationConversion;

	bool firsTimeInit; // Необходимость дождатся загрузки маршрута в колонну.
	float speedAtThisWp; // Скорость на текущей точке маршрута.
	float speedAtNextWp; // Скорость на следущей точке маршрута.
	bool firstAcc; // Флаг необходимости разгона из состояния STOP.
	int nextWp; // Id следующей wp.
	int lastWp; // Id крайней wp.

private:
	int walkAnimationArgument;

	float walkStartDistance;
	float walkStartBegin;
	float walkStartEnd;

	float walkCycleDistance;
	float walkCycleBegin;
	float walkCycleEnd;

	float walkStopDistance;
	float walkStopBegin;
	float walkStopEnd;

	float walkToRunDistance;
	float walkToRunBegin;
	float walkToRunEnd;

	int botRunAnimantionArgument;
	int humanRunAnimationArgument;

	float runStartDistance;
	float runStartBegin;
	float runStartEnd;

	float runCycleDistance;
	float runCycleBegin;
	float runCycleEnd;

	float runStopDistance;
	float runStopBegin;
	float runStopEnd;

	float runToWalkDistance;
	float runToWalkBegin;
	float runToWalkEnd;

	int deadAnimationArgument;

	float deadAnimationBegin;
	float deadAnimationEnd;
	float deadAnimationLength;
};