#ifndef __HydraulicBubbleCamera_h__
#define __HydraulicBubbleCamera_h__

#include "Base.h"

namespace EagleFM
{

//Гидравлический цилиндр. Определяет объем жидкости по расходу во времени и расчитывает давление (линейно)
class FMBASE_API HydraulicBubbleCamera
{
public:
	HydraulicBubbleCamera(double PressureMax = 1000000, double VolumeMax = 0.0001); 
	void	init(double PressureMax, double VolumeMax);

	void	simulate(double dt);

	double	getPressureMax()const { return PressureMax; }
	double	getVolumeMax()	const { return VolumeMax; }
	double	getPressure()	const { return Pressure; }
	double	getVolume()		const { return Volume; }

	double	dVolume;		//расход гидрожидкости

private:
	//Характеристики
	double	PressureMax;	
	double	VolumeMax;

	//Текущие параметры
	double	Pressure;
	double	Volume;
};

}

#endif