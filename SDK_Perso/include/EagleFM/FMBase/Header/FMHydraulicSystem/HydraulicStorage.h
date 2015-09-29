#ifndef __HydraulicStorage_h__
#define __HydraulicStorage_h__

#include "base.h"

namespace EagleFM
{

//Гидробак
class FMBASE_API HydraulicStorage
{
public: //temp
	HydraulicStorage(double BP = 0.0, double VFull = 0.01);
	
	void	init(double BP, double VFull = 0.01);

	void	simulate(double dt);

	double	getVolume()			const { return Volume; }
	double	getBoostPressure()	const { return BoostPressure; } 
	
	double	dVolume;		//расход гидрожидкости

private:
	double	Volume;			//текущий объем гидрожидкости

	// ТХ
	double	BoostPressure;	//давление наддува (определяет давление в магистрали слива)	
	double	VolumeFull;		//полный (рекомендуемый) объем гидрожидкости
};

}

#endif