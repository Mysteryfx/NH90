#ifndef __HydraulicDumpValve_h__
#define __HydraulicDumpValve_h__

#include "Base.h"

namespace EagleFM
{

//Клапан для слива гидрожидкости из магистрали нагнетания в магистраль слива
class FMBASE_API HydraulicDumpValve
{
public:
	HydraulicDumpValve(double ValveDeliverCapacity = 2.5e-7, double ValveSinkCapacity = 2.5e-7); 

	void	init(double ValveDeliverCapacity, double ValveSinkCapacity);

	//расчет давления за клапаном и расхода через него по давлениям нагнетания и слива
	void	calcPressureAndConsumption(double PressureDeliver, double PressureSink, double* PressureOut, double* Consumption);

	bool	Open;					// состояние: true - слив открыт, false - закрыт

private:
	//	характеристики
	double	ValveDeliverCapacity;	//проводимость нагнетания золотника
	double	ValveSinkCapacity;		//проводимость слива золотника

	//кэш
	double	ValveDeliverCapacity2;
	double	ValveSinkCapacity2;	
};
	
}

#endif