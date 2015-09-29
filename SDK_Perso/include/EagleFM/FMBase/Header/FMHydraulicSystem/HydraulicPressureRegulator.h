#ifndef __HydraulicPressureRegulator_h__
#define __HydraulicPressureRegulator_h__

#include "Base.h"

namespace EagleFM
{

//Регулятор давления. Определяет расход по заданному давдению на выходе, положению золотника и располагаемым давлениям нагнетания, слива, выхода
class FMBASE_API HydraulicPressureRegulator
{
public:
	HydraulicPressureRegulator(double PressureOutMax = 1e6, double ValveDeliverCapacityMax = 8e-8, 
		double ValveSinkCapacityMax = 8e-7); 
	
	void	init(double PressureOutMax, double ValveDeliverCapacityMax = 8e-8, //макс.давление после регулятора, макс.проводимость нагнетания золотника
		double ValveSinkCapacityMax = 8e-7); //макс.давление после регулятора, макс.проводимость слива золотника

	void	setRelativeControl(double Cntrl) { Control = __max(0.0, __min(Cntrl, 1.0)); }

	//расчет расходов подачи и слива (ConsumptionDeliver, ConsumptionSink) по золотнику (Control), давлению нагнетания
	void	calcConsumptions(double PressureDeliver, double PressureSink, double PressureOut, double* ConsumptionDeliver, double* ConsumptionSink);

	double	getRelativeControl() const { return Control; }


private:
	double	PressureOutMax;				//заданное давление на выходе
	double	ValveDeliverCapacityMax;	//макс.проводимость нагнетания золотника
	double	ValveSinkCapacityMax;		//макс.проводимость слива золотника

	double	Control;					// положение золотника 0...1
};

}

#endif