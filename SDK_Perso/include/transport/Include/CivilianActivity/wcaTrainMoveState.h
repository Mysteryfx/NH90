#pragma once
#include "CivilianActivity/wcaCarMoveState.h"
class wcaTrain;

class wcaTrainMoveState: public wcaCarMoveState
{
public:
	wcaTrainMoveState(wcaCar* host);
	virtual ~wcaTrainMoveState();

	double GetPathPar(wModelTime t) const;
	void update();

private:
	wModelTime timeOnStop(wModelTime t) const;

	wcaTrain *prev;
	float prev_dist;
	wModelTime oneWayTime, delayTime;
};