#ifndef __HydraulicDrive_h__
#define __HydraulicDrive_h__

#include "FMMathematics/math.h"
#include "Base.h"

namespace EagleFM
{
struct FMBASE_API HydraulicDriveInit
{
	HydraulicDriveInit()
	{
		PressureUp = 100*PressureMCA_SL;
		PressureDn = PressureMCA_SL;
		PlungerSpeedMax = 0.1;
		PlungerPosMax = 0.1;	
		ExternalForceMax = 1000.0*9.8;
		ExternalForceMin = 100.0*9.8;
		PlungerArea = 0.0;
		Servo = false;			
		K_Servo = 100;	
		BypassPressure = 0.0;
		K_BypassCapacity = 10.0;
		PressureLimit = 10000*PressureMCA_SL;
	}

	double	PressureUp;			//рабочее давление
	double	PressureDn;			//давление слива
	double	PlungerSpeedMax;	//макс. скорость перемещения штока
	double	PlungerPosMax;	
	double	ExternalForceMax;	//макс.усилие (для расчета характеристик, если не задана площадь поршня)
	double	ExternalForceMin;	//порог внешнего усилия для передвижения штока
	double	PlungerArea;		//площадь поршня (ВНИМАНИЕ! если не задано - вычисляется по макс.усилию!!!)
	bool	Servo;			
	double	K_Servo;	
	double	BypassPressure;		//пороговое давление, ниже которого включается кольцевание
	double	K_BypassCapacity;	//к-т увеличения проводимости при кольцевании
	double	PressureLimit;		//порог предохранителя от внутреннего давления
};

// Гидропривод. Используя информацию о распалогаемом давлении и положении золотника
// по своим характеристикам рассчитывает перемещение штока. 
class FMBASE_API HydraulicDrive // Optimizable???
{
//----------------------	МЕТОДЫ	----------------
public:
	HydraulicDrive();
	HydraulicDrive(const HydraulicDriveInit&);
	void	init(const HydraulicDriveInit&);
	void	initState(double PlungerReletaivePos) { PlungerPos = PlungerPosMax*PlungerReletaivePos/2.0; }

	void	setControlLim(double Cntrl) { ControlLim = __max(0.0, __min(Cntrl, 1.0)); setRelativeControl(Control); }
	void	setRelativeControl(double Cntrl) { Control = __max(-ControlLim, __min(Cntrl, ControlLim)); }
	void	setPlungerPos(double Pos){PlungerPos=Pos;}
	void	simulate(double dt);

	double	getPlungerPos()								const { return PlungerPos + PlungerPosMax/2.0; } // 0 ... PlungerPosMax
	double	getPlungerRelativePos(bool half = false)	const { return !half ? 2*PlungerPos/PlungerPosMax : PlungerPos/PlungerPosMax + 0.5; } // -1 ... 1  либо 0 ... 1
	double	getPlungerSpeed()							const { return PlungerSpeed; } 
	double	getPlungerRelativeSpeed()					const { return PlungerSpeed/PlungerPosMax; } 
	double	getConsumption()							const { return Consumption; }
	double	getRelativeControl()						const { return Control; }
	double	getControlLim()								const { return ControlLim; }

//------------------------	ПОЛЯ	------------------
	double	ExternalForce;		//внешняя нагрузка
	double	DiffPressure;		//перепад давлений
	bool	Freeze;				//фиксация по внешнему условию
	bool	Bypass;				//принудительное кольцевание извне

private:
	double	Control;			//управление (-1 ... 1)
	double	ValvePos;			//положение золотника (-1 ... 1)
	double	PlungerPos;			//положение штока привода (-PlungerPosMax/2 ... PlungerPosMax/2)
	double	PlungerSpeed;		//скорость перемещения штока
	double	Consumption;		//расход
	bool	PressLimitOn;		//состояние предохранителя от внутреннего давления (от внешнего усилия)

	// ТХ
	double	PlungerPosMax;
	double	PlungerArea;
	double	ValveCapacityMax;	//макс.проводимость золотника
	bool	Servo;				//true - привод следящий
	double	K_Servo;			//коэф-т слежения
	double	BypassPressure;		//пороговое давление, ниже которого включается кольцевание
	double	K_BypassCapacity;	//к-т увеличения проводимости при кольцевании
	double	ExternalForceMin;	//порог внешнего усилия для передвижения штока
	double	PressureLimit;		//порог предохранителя от внутреннего давления

	double	ControlLim;			//0...1
};

}

#endif