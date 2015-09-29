//             file  wsFlightPlan.h
#ifndef __wsFlightPlan_h__
#define __wsFlightPlan_h__

const unsigned int RW1 = 0;
const unsigned int RW2 = 1;

#include "WorldGeneral.h"
#include "Serializer\Serializer.h"
#include "Serializer\Serializable.h"
#include "cLinear.h"
#include "wTime.h"

struct Waypoint    // Описывает точку маршрута. Связный список ППМ. 
{
	float			point[3];	// Координаты радиуса-вектора точки : x, y, z.
	bool			ralt;		// Радиовысота
	float			V_target;   // Заданная скорость в точке
	wModelTime      ETA;		//Время прибытия
	bool			V_locked;	//Скорость задана
	bool			ETA_locked;	//Время прибытия задано
	unsigned char	Fly_over;   // действие в точке
	unsigned long	PointNum;	// 	порядковый номер точки (от 0)
	int				NextPoint;	// 	номер следующей точки (-1 - конец маршрута)
	unsigned long	mId;		// Идентификатор точки в миссии
    ed::string     name;       // имя точки заданное в редакторе
	unsigned long	idRoute;		// идентификатор для unitslayer (используется только там)
	Waypoint*		pNext;     // next waypoint in list
	Waypoint*		pPrev;     // prev waypoint in list (optional , added for reverse iteration in FC3)

	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, Waypoint& data);
}; 
                           
struct TaxiPoint  
{  
	double			point[3]; 
	double			V_target; 
	TaxiPoint*		pNext; 
	unsigned char	Fly_over; 
	unsigned long	idRoute;  		// идентификатор для unitslayer (используется только там)

	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, TaxiPoint& data);
}; 
                          //  В последнем  поле pNext содержит 0.  
struct TaxiRoute // Описывает маршрут рулежки до места стоянки.  
{ 
	unsigned char	TaxiName; // Имя (номер) маршрута.
	TaxiPoint*		pTRoute;      // Указатель на маршрут рулежки.
};

struct OrbitParam //Параметры барражирования
{
	enum Type
	{
		ORBIT_NO,	
		ORBIT_CIRCLE,
		ORBIT_RACE_TRACK,
		ORBIT_CIRCLE_AROUND_POINT
	};
	OrbitParam() : type(ORBIT_NO), flags(0) {;}
	enum Flags
	{
		FLAG_NO = 0,
		FLAG_ORBIT_POINT = 1 << 0,
		FLAG_ORBIT_SPEED = 1 << 1,
		FLAG_ORBIT_ALTITUDE = 1 << 2,
	};
	Type		type;
	E2::Vector	point;
	E2::Vector	point2;
	unsigned int flags;
	float		speed;
	float		altitude;
};

struct GroundLandingParam
{
	GroundLandingParam() : flag(false) {;}
	bool flag;
	E2::Vector point;
};

#endif
