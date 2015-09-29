#pragma once

#include "FMSpace/AirFlow.h"

//"Кирпичики" реактивного двигателя любого типа:
//	- входное устройство 
//	- компрессор
//	- камера сгорания
//	- турбина
//	- сопло

//	Компрессор и турбина требуют подробные данные:
//	компрессор - в виде диаграммы "Степень сжатия = f(привед.обороты,привед.расход воздуха)",
//	турбина - зависимость "Привед.расход воздуха = f(Степень расширения)" и эллипс "кпд = f(норм.степ.расш., норм.привед.обороты)"
//  и используются для построения физичных моделей (основанных на физике процесса протекания воздуха по тракту)
//	реактивного (турбореактивного) двигателя


namespace EagleFM {

struct AirFlow;

class  IJetElement //общий интерфейс
{
public:
	IJetElement() { AirFlowExt_.Temperature = 288.0; }
	virtual	~IJetElement() {}

	void	setExternAirFlow(const AirFlow& air_flow) { AirFlowExt_ = air_flow; }
	void	resetExternAirFlow() { 
		AirFlowExt_.Mass = 0.0; 
		AirFlowExt_.Temperature = 288.0; 
		AirFlowExt_.Pressure = PressureMCA_SL;	
	}

protected:

	AirFlow AirFlowExt_; //К любому элементу можно подвести наружный воздух
};

}