#pragma once

#include "Base.h"
#include "FMMathematics/math.h"
#include "FMSensorsSystem/IPito.h"

namespace EagleFM
{

class DynamicBody;

class FMBASE_API Pito : public IPitotStatic
{
public:
	Pito(DynamicBody* const, double DiamIn, double LengthIn, double EmpiricIn);

	void			setHeatPower(double Power) { HeatPower = __max(0.0, Power); }
	void			setIF(double _IF) { IF = __max(0.0, __min(_IF, 1.0)); }
    
	void			simulate(double dt);

	double			getIF()					const { return IF; }

	// Доступ к измеряемым величинам
	virtual double	getAtmoPressure()		const { return AtmoPressure; }
	double			getAtmoTemperature()	const { return AtmoTemperature; }
	double			getTrueAirSpeed()		const { return TrueAirSpeed; }
	double			getIndicatedAirSpeed()  const { return IndicatedAirSpeed; }
	virtual double	getVerticalSpeed()		const { return VerticalSpeed; }
	double			getMach()				const { return Mach; }
	double			getIceLayerThickness()	const { return IceLayerThickness; }
	double			getBodyTemperature()	const { return BodyTemperature; }
	double			getTauDynByIce()		const { return TauDynByIce; }
	double			getTauStatByIce()		const { return TauStatByIce; }


	Vec3			Pos_l;					// Координаты ПВД
	double			Elevation_l;			// Угол установки ПВД
	bool			DynamicSlowdownFlag;	// Учет динамического сжатия на большой скорости

	Vec3			SurfacePoint_w;		//точка на поверхности под ЛА (для коррекции ветра)
	Vec3			SurfaceNormal_w;	//нормаль к точке на поверхности


private:
	DynamicBody* const  pDBody;

	double			AtmoPressure;			// Статическое давление
	double			AtmoTemperature;		// Температура
	double			TrueAirSpeed;			// Воздушная скорость
	double			IndicatedAirSpeed;		// Индикаторная скорость
	double			VerticalSpeed;			// Вертикальная скорость
	double			Mach;					// Число М

	double			IF;

	double			FullPressureSensor;
	double			StaticPressureSensor;

	//лед
	double			IceLayerThickness;		//толщина слоя льда
	double			HeatPower;				//мощность обогревателя
	double			BodyTemperature;		//температура ПВД
	double			TauDynByIce;			//запаздывание замера дин.давления от обледенения (нет обледениния: = 1)
	double			TauStatByIce;			//запаздывание замера стат.давления от обледенения (нет обледениния: = 1)

	double			Diam;					// Диаметр штанги (для расчета температуры)
	double			Length;					// Длина штанги (для расчета температуры)
	double          Empiric;
};
}
