#ifndef __HydraulicAutoDrive_h__
#define __HydraulicAutoDrive_h__

#include "FMHydraulicSystem/HydraulicDrive.h"

namespace EagleFM
{
// Гидропривод со встроенным гидроприводом автопилота (используется на вертолетах). 
// Гидропривод автопилота управляется электрическими сигналами с АП и своим штоком 
// добавляет сигнал в управление основного гидропривода.

class FMBASE_API HydraulicAutoDrive
{
//----------------------	МЕТОДЫ	----------------
public:
	HydraulicAutoDrive();
	HydraulicAutoDrive(const HydraulicDriveInit&, double AutoPlungerPosMax, double AutoPlungerSpeedMax);
	void			init(const HydraulicDriveInit&, double AutoPlungerPosMax, double AutoPlungerSpeedMax, double AutoRelativeBias = 0.0);
	void			initState(double PlungerRelativePos) { MainDrive.initState(PlungerRelativePos); FeedBackDrive.initState(PlungerRelativePos);	}
	void            setPlungerPos(double Pos){ AutoDrive.setPlungerPos(Pos);MainDrive.setPlungerPos(Pos);FeedBackDrive.setPlungerPos(Pos);}

	void			setControlLim(double Cntrl) { MainDrive.setControlLim(Cntrl); FeedBackDrive.setControlLim(Cntrl); }
	void			setRelativeControl(double Cntrl) { Control = __max(-1.0, __min(Cntrl, 1.0)); }
	void			setRelativeAutoControl(double Cntrl) { ControlAuto = __max(-1.0, __min(Cntrl + AutoBias, 1.0)); }

	void			simulate(double dt);

	double			getPlungerPos()								const { return MainDrive.getPlungerPos(); }
	double			getPlungerRelativePos(bool half = false)	const { return MainDrive.getPlungerRelativePos(half); }
	double			getPlungerSpeed()							const { return MainDrive.getPlungerSpeed(); } 
	double			getPlungerRelativeSpeed()					const { return MainDrive.getPlungerRelativeSpeed(); } 
	double			getFeedBackRelativePos(bool half = false)	const { return FeedBackDrive.getPlungerRelativePos(half); } //положение рычага управления (обратная связь)
	double			getConsumption()							const { return MainDrive.getConsumption() + AutoDrive.getConsumption(); }
	double			getRelativeControl()						const { return Control; }
	double			getRelativeAutoControl()					const { return ControlAuto; }
	double			getControlLim()								const { return MainDrive.getControlLim(); }
	double			getAutoPlungerRelativePos()					const { return AutoDrive.getPlungerRelativePos(); }

//------------------------	ПОЛЯ	------------------
	double			ExternalForce;		//внешняя нагрузка
	double			DiffPressure;		//перепад давлений
	bool			Freeze;				//фиксация по внешнему условию
	bool			Bypass;				//принудительное кольцевание извне

private:
	HydraulicDrive	AutoDrive;
	HydraulicDrive	MainDrive;
	HydraulicDrive	FeedBackDrive; //фиктивный привод! для расчета обратной связи (без учета воздействия автопилота)
	double			K_AutoRelative;
	double			AutoBias;			

	double			Control;			//управление (-1 ... 1)
	double			ControlAuto;		//управление автопилота (-1 ... 1)
};

}

#endif