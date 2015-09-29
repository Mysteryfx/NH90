#ifndef wcaRouteConst_H
#define wcaRouteConst_H

#include <ed/string.h>
#include <ed/list.h>
#include <ed/map.h>

#include "wsType.h"

enum wcaRouteType
{
	rtRoad,
	rtRail,
	rtSea,
	rtRiver,
	rtAir
};

// описание маршрута
struct wcaRouteDescriptor
{
	typedef ed::list<wsType> typesList;

	wcaRouteType routeType;		// тип маршрута (класс)
	
	ed::string name;			// название типа маршрута
	typesList types;			// перечень машинок, ездеющих по нему
	double speed;				// скорость, в м/с
	double stop_delay;			// время ожидания на остановке
	
	ed::map<ed::string, double> period; // время между проходами машинок, ключ - строка из опций
	double period_distortion;	// дисперсия времени между проходами машинок
	
	int wagons_min, wagons_max;	// для поездов: количество вагонов
	typesList locomotives;		// для поездов: список возможных локомотивов
};

#endif // wcaRouteConst_H