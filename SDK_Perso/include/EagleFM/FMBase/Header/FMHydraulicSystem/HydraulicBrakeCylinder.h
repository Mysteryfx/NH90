#ifndef __HydraulicBrakeCylinder_h__
#define __HydraulicBrakeCylinder_h__

#include "Base.h"

#include "FMMathematics/Constants.h"

namespace EagleFM
{

//Классический тормозной цилиндр. Определяет давление по золотнику и давлению нагнетания. Расход разовый в самом начале хода золотника
class FMBASE_API HydraulicBrakeCylinder
{
public:

	enum enum_BrakeCylinder_Failures
	{
		FAULT_BRAKE_NO = 0,
		FAULT_BRAKE_TOTAL
	};

	HydraulicBrakeCylinder(double pressure0 = 10*PressureMCA_SL, double volume0 = 0.0001); 
	
	void	init(double pressure0, double volume0 = 0.0001); //рабочее (макс.) давление колодок, разовый расход
	
	void	setRelativeControl(double control) { _control = __max(0.0, __min(control, 1.0)); }

	void	simulate(double dt);

	double	getRelativeControl()	const { return _control; }
	double	getPressure()			const { return _pressure; }
	double	getRelativePressure()	const { return _pressure/_pressure0; }
	double	getConsumptionDeliver()	const { return _consumptionDeliver; }
	double	getConsumptionSink()	const { return _consumptionSink; }
	double  getMaxPressure()		const { return _pressure0; } 

	void set_failure(unsigned int failure)
	{
		failure_ = failure;
	}

	void repair()
	{
		failure_ = FAULT_BRAKE_NO;
	}

	double	pressureDeliver;		//давление нагнетания
	double	volumeDeliver;			//располагаемая жидость в линии нагнетания

private:
	double	_pressure0;				//заданное давление (перепад от атмосферного!)
	double	_pressure;				//текущее давление (перепад от атмосферного!)
	double	_volume0;				//объем цилиндра
	double	_volume;				//текущий объем 
	double	_consumptionDeliver;	//текущий расход 
	double	_consumptionSink;		//текущий слив

	double	_control;				//положение золотника 0...1

	unsigned int failure_;
};

}

#endif