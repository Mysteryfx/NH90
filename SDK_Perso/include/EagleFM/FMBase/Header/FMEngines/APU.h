#pragma once

#include "Base.h"

#include "FMEngines/JetEngine.h"
#include "FMEngines/JetEngineElements.h"
#include "FMEngines/Compressor.h"

namespace EagleFM
{

// Модель типой ВСУ, позволяющая получить горячий воздух, отбираемый за компрессором.

class FMBASE_API APU : public JetEngine
{
// ----------------------------- МЕТОДЫ ---------------------------------------

public:
	APU(DynamicBody *DBody, FuelPipeline *pFPipeline = 0, double dt = 0.04);
	virtual ~APU() {}

	void						setBleedAirMassFlow(double BleedAMF) { BleedAirMassFlow = BleedAMF; }

protected:
	virtual void				doSimulate();

	virtual double				calcGG_Moment() { return GG_Moment; }


	//	Доступ:
public:
	
	virtual void				getBlowerFlow(double &Temperature, double &Pressure) const;

	virtual double				getParam(int param) const;
// ----------------------------- ПОЛЯ ---------------------------------------    

protected:
	//	Элементы ВСУ
	JetInlet					Inlet;					//воздухозабрник
	JetCompressor				Compressor;				//комрпессор
	JetCombustor				Combustor;				//камера сгорания
	JetTurbine					GG_Turbine;				//турбина компрессора
	JetCompressorNozzleVector	CompressorNozzles;		//в конкретном двигателе можно добавить любое отверствие в компрессор (m = 0...1 - расположение отверствия "до компрессора"..."после")

	//	Характеристики ВСУ
	double						PressureDiffusorRatio;	//отношение давлений на входе и выходе из диффузора
	double						GG_OmegaTerminal;		//до этих оборотов параметры компрессора лежат на рабочей линии 
	double						StallAirFlowK;			//коэф-ты ухудшения параметров при неустойчивой работе компрессора
	double						StallPiK;				
	double						StallNuK;
	double						GG_FrictionMoment;		//момент трения на валу


	//	Параметры работы
	AirFlow						EngineAirFlow;			//характеристики воздушного потока (меняются по тракту)
	double						Beta;					//текущее отношение степени сжатия компрессора к расходу воздуха (используется как независимая переменная при решении уравнения)
	double						TemperatureH;			//температура за бортом
	double						PressureH;				//давление на высоте
	double						Temperature2;			//температура после компрессора
	double						Pressure2;				//давление после компрессора
	double						Temperature3;			//температура после камеры сгорания
	double						Pressure3;				//давление после камеры сгорания
	double						Temperature4;			//температура после турбины компрессора
	double						Pressure4;				//давление после турбины компрессора
	double						BleedAirMassFlow;		//отбираемый расход воздуха

	double						SurgeThrottle;			//дроссель помпажа (0 - нет ... 1 - помпаж, но вообще подбирается по конкретному двигателю)

	double						GG_Moment;				//крутящий момент на валу газогенератора

	double						TurbinePi;				//степень расширения турбины
	double						TurbineNu;				//КПД турбины
};

}