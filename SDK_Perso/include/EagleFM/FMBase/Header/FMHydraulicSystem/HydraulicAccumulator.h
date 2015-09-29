#ifndef __HydraulicAccumulator_h__
#define __HydraulicAccumulator_h__

#include "base.h"
#include <ed/vector.h>

namespace EagleFM
{

class HydraulicAccumulator;

struct HydraulicAccumulatorJoin //структура для описания дочерних аккумуляторов
{
	HydraulicAccumulator*	pHydraulicAcc;			//присоединенный аккумулятор
	double					DeltaPressureTerminal;	//перепад давления на клапане
};

//Гидроаккумулятор, определяющий распологаемое давление в ГС по объему гидрожидкости в аккумуляторе
class FMBASE_API HydraulicAccumulator
{
public: 
	HydraulicAccumulator(double PMax = 1e7, double PInit = 0.0, double VFull = 0.01);
	void		init(double PMax, double PInit = 0.0, double VFull = 0.01);
	void		join(HydraulicAccumulator*, double DeltaPressureTerminal = 0.0); //добавить нахлебника (давление зарядки у всех должно быть одинаковым!)
	void		setChildDeltaPressTerminal(int i, double DeltaPressureTerminal); //изменить предел перепада на обратном клапане у нахлебника i
	void		setChild(bool);

	void		setVolume(double V); 

	void		simulate(double dt);

	double		getPressure()		const { return Pressure; }
	double		getPressureInit()	const { return PressureInit; }
	double		getPressureMax()	const { return PressureMax; }

	double		getVolume()			const { return Volume; }
	double		getVolumeFull()		const { return VolumeFull; }
	double		getVolumeMax()		const { return VolumeMax; }

	double		dVolume;		//расход гидрожидкости

private:
	double		Pressure;		// давление в аккумуляторе
	double		Volume;			// объем гидрожидоскти в аккумуляторе

	// Характеристики
	double		PressureMax;	// рабочее давление
	double		PressureInit;	// давление зарядки аккумулятора	
	double		VolumeFull;		// объем гидробака (в куб.метрах)
	double		VolumeMax;		// рабочий объем жидкости (вычисл. по рабочему давл.)

	//Нахлебники
	typedef ed::vector<HydraulicAccumulatorJoin> HydraulicAccumulatorJoinVector;
	HydraulicAccumulatorJoinVector	JoinAccumulators;		//соединенные с данным аккумуляторы
	int								JoinNum;				//кол-во присоединенных ГА
	bool							IsChild;				//является нахлебником
};

}

#endif