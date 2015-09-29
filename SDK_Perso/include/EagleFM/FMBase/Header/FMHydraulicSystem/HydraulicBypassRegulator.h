#ifndef __HydraulicBypassRegulator_h__
#define __HydraulicBypassRegulator_h__

#include "Base.h"

namespace EagleFM
{

//Регулятор давления. Определяет расход по заданному давдению на выходе, положению золотника и располагаемым давлениям нагнетания, слива, выхода
class FMBASE_API HydraulicBypassRegulator
{
public:
	HydraulicBypassRegulator(double pressure0 = 1e6, double valveCapacityMax = 8e-8); 
	
	void	init(double pressure0, double valveCapacityMax); //заданное макс.давление после регулятора, макс.проводимость золотника
	
	void	setRelativeControl(double control) { _control = __max(0.0, __min(control, 1.0)); }

	//расчет давления и расхода по золотнику (Control), давлению нагнетания и слива
	void	calcPressureAndConsumption(double pressureDeliver, double pressureSink);

	double	getRelativeControl()	const { return _control; }
	double	getPressure()			const { return _pressure; }
	double	getRelativePressure()	const { return _pressure/_pressure0; }
	double	getConsumption()		const { return _consumption; }


private:
	double	_pressure0;					//заданное давление
	double	_pressure;					//текущее давление
	double	_valveCapacityMax;			//макс.проводимость
	double	_consumption;

	double	_control;					// положение золотника 0...1
};

}

#endif