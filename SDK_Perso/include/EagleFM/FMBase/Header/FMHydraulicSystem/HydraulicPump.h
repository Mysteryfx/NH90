#ifndef __HydraulicPump_h__
#define __HydraulicPump_h__

#include "base.h"

namespace EagleFM
{


//Гидронасос, накачивающий давление в ГС
class FMBASE_API HydraulicPump
{
public:
	HydraulicPump(double PMax = 1e7, double DFactor = 0.1, double K_S = 0.7);
	void			init(double PMax, double DFactor = 0.1, double K_S = 0.7);

	
	void			simulate(double dt);
	double			getProductivity() const { return Productivity; }	
	


	double			RPM;
	double			Pressure;			//давление в гидроаккумуляторе
	double			Volume;				//объем жидкости в гидробаке

private:
	double			Productivity;		//производительность насоса

	//	Характеристики
	double			PressureMax;		//рабочее давление
	double			DisplacementFactor; //коэф-т производительности по оборотам
	double			K_Servo;			//порог перепада для отслеживания шайбой насоса (производительности)

};

}

#endif