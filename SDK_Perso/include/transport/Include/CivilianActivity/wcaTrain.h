#ifndef wcaTrain_H
#define wcaTrain_H

#include "wcaCar.h"
class wcaMirageTrains;

// Миражный поезд
// Отличается от машинок только жестким соединением с паровозом
class wcaTrain : public wcaCar
{
public:
	wcaTrain(wcaMirageTrains*_route, double _initpar, wsType _type, wcaTrain *_prev = 0);

protected:
	void procDeath(wModelTime at, ObjectID hitman);

private:
	// предыдущий вагон(локомотив)
	wcaTrain *prev;

	friend class wcaTrainMoveState;
};

#endif // wcaTrain_H
