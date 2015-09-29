#ifndef __Damper_h__
#define __Damper_h__

#include "Base.h"

namespace EagleFM
{

//Модель демпфера, сутью которой является расчёт силы реакции демпфера
//в зависимости от скорости перемещения штока. 
//Применяется при «сборке» амортизатора.
class FMBASE_API Damper
{
public:
	Damper(double _ForceFactor = 0.0) : ForceFactor(_ForceFactor), Speed(0.0), Force(0.0) {}

	// Расчет силы и скорости
	inline double calcForceBySpeed();	// Расчет силы по скорости
	inline double calcSpeedByForce();	// Расчет скорости по силе
	inline double calcForceBySpeed(double Speed);	// Расчет силы по скорости
	inline double calcSpeedByForce(double Force);	// Расчет скорости по силе

	double ForceFactor;			// Коэффициент демпфирования
	
	double Speed;				// Скорость
	double Force;				// Сила
	
};

}

#endif