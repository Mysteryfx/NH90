#pragma once

#include "Base.h"

#include "FMEngines/JetEngine.h"
#include "FMEngines/JetEngineElements.h"
#include "FMEngines/Compressor.h"

namespace EagleFM
{

// Модель типового ТВД, позволяющая получить момент на свободной турбине.
// Основана на поиске решением уравнений по текущим оборотам вала расхода воздуха через весь тракт. 
// Решение организовано следующим образом:
// по приведенным оборотам и параметрам воздуха после воздухозаборника
// в компрессоре находится кривая "Степень сжатия = f(Расход воздуха)",
// далее фиксируется расход воздуха (при первом обращении по рабочей линии, затем из предыдущего шага),
// получаются параметры воздуха за компрессором, затем за камерой сгорания, далее решается уравнение 
// по сведению баланса между турбиной компрессора и свободной турбиной (внутренний цикл:
// разность между расходами воздуха в турбинах сводится к нулю изменением степени расширения
// на турбине комрессора и поиском производной этой разницы), находится расход
// воздуха в турбинах. Разница (Дельта) между этим расходом воздуха и тем, что был зафиксирован в компрессоре
// сводится к нулю во внешнем цикле путем приращения заданного расхода воздуха в компрессоре, поиска производной 
// Дельты (реакции на приращение) и определения баланса с учетом производной.

class FMBASE_API TurboShaft : public JetEngine
{
// ----------------------------- МЕТОДЫ ---------------------------------------
public:
	TurboShaft(DynamicBody *DBody, FuelPipeline *pFPipeline = 0, double dt = 0.04);
	virtual ~TurboShaft() {}

	void						setPT_RPM(double RPM) { PT_OmegaExternal = RPM/Omega_to_RPM; }


protected:
	virtual void				doSimulate();

	virtual double				calcGG_Moment() { return GG_Moment; }

	virtual double				calcComprBleedAMF(double ComprPi, double ComprNu);

	//				расчет расхода воздуха через турбины при данных параметрах потока за КС (GGT_PiReq - стартовое значение для решения уравнения)
	double						calcTurbinesGasFlow(const AirFlow&, double &GGT_Pi, double &PT_Pi, double GGT_PiReq = -1.0); 

	//	Доступ
public:
	double						getTurbineRPM()		const { return PT_Omega*Omega_to_RPM; }
	virtual double				getTurbineMoment()	const { return PT_Moment; }
	double						getTurbineMOI()		const { return PT_MOI; }
	double						calcFuelHeat()		const { return Combustor.calcFuelHeat(); }

	
// ----------------------------- ПОЛЯ ---------------------------------------    

protected:
	//	Элементы ТВД
	JetInlet					Inlet;					//воздухозабрник
	JetCompressor				Compressor;				//комрпессор
	JetCombustor				Combustor;				//камера сгорания
	JetTurbine					GG_Turbine;				//турбина компрессора
	JetTurbine					PowerTurbine;			//свободная турбина
	JetCompressorNozzleVector	CompressorNozzles;		//в конкретном двигателе можно добавить любое отверствие в компрессор (m = 0...1 - расположение отверствия "до компрессора"..."после")

	//	Характеристики ТВД
	double						PT_MOI;					//момент инерции свободной турбины
	double						PressureDiffusorRatio;	//отношение давлений на входе и выходе из диффузора
	double						GG_OmegaTerminal;		//до этих оборотов параметры компрессора лежат на рабочей линии 
	double						StallAirFlowK;			//коэф-ты ухудшения параметров при неустойчивой работе компрессора
	double						StallPiK;				
	double						StallNuK;

	
	//	Параметры работы
	double						PT_Omega;				//полученная частота вращения свободной турбины без зацепления с редуктором
	double						PT_OmegaExternal;		//заданная частота вращения СТ при зацеплении

	AirFlow						EngineAirFlow;			//характеристики воздушного потока (меняются по тракту)
	double						TemperatureH;			//температура за бортом
	double						PressureH;				//давление на высоте
	double						Temperature1;			//температура после воздухозаборника
	double						Pressure1;				//давление после воздухозаборника
	double						Temperature2;			//температура после компрессора
	double						Pressure2;				//давление после компрессора
	double						Temperature3;			//температура после камеры сгорания
	double						Pressure3;				//давление после камеры сгорания
	double						Temperature4;			//температура после турбины компрессора
	double						Pressure4;				//давление после турбины компрессора

	double						SurgeThrottle;			//дроссель помпажа (0 - нет ... 1 - помпаж, но вообще подбирается по конкретному двигателю)

	double						GG_Moment;				//крутящий момент на валу газогенератора

	double						PT_Moment;				//крутящий момент на свободной турбине (цель расчетов модели)


// ----------------------------- DEBUG и TEMP ---------------------------------------    
};



//Модель редуктора ТВД, обеспечивающая связь ТВД с винтом.
class Rotor;

class FMBASE_API RotorGear : public Optimizable
{
public:
	RotorGear(Rotor*, TurboShaft*, TurboShaft*, double N, double dt = 0.006); //двигатели - Temp!
	virtual ~RotorGear(){}

private:
	virtual void	doSimulate();
public:

	double			getRPM()			const;
	double			getReductorMoment()	const { return ReductorMoment; }
	double			getReductorMOI()	const { return ReductorMOI; }

	double			getReductorN()		const { return ReductorN; }

	bool			LeftLockBroken;		//разрушение механизма сцепки СТ лев. двигателя с редуктором
	bool			RightLockBroken;	// .. прав. ..

private:
	Rotor			*pRotor;
	//TurboShaft		*aTurboShast;	//массив двигателей
	//bool				*Locks; //массив флагов сцепок

	double			ReductorMoment;
	double			ReductorMOI;
	
	double			ReductorN;		//передаточное число

	bool			LeftLock;		//Temp: сцепка с левым двигателем
	bool			RightLock;

public: //TEMP!!! 
	TurboShaft		*pLeftEngine;	
	TurboShaft		*pRightEngine;	


};

}