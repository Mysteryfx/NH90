#ifndef wcaMirageTrains_H
#define wcaMirageTrains_H

#include "Transport.h"
#include "wcaMirageActivity.h"
#include "Randomizer\RandomObject.h"

class wcaTrain;

// деятельность - поезд
class TRANSPORT_API wcaMirageTrains : public wcaMirageActivity
{
protected:
	// количество вагонов в поезде
	int wagons;

	// последний созданный вагон
	wcaTrain *last;

	// получить максимальную ширину и радиус разворота
	virtual void GetStats();
public:
	static RandomObject randomEvenly;

	wcaMirageTrains(int wagon_count = 10);

	float trainLength() const;
	// инициализация маршрута
	virtual void Init(const wcaRouteDescriptor &desc);

	// загрузить маршрут из r-файла
	virtual bool LoadRoute(NM_ROAD::Road& road);

	void stopAllTrains();

	// создание машинки (передается номер и начальный параметр)
	virtual void CreateCars(int count);
	virtual wcaCar *CreateCar(int n, double initpar);
};

#endif // wcaMirageTrains_H